# This program is free software: you can redistribute it and/or modify  
# it under the terms of the GNU General Public License as published by  
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with this program. If not, see <http://www.gnu.org/licenses/>.

import sys
import warnings
import argparse
from collections import deque


class NodeProgram:
    """Top-node of the program

    Holds functions and global variables.
    """
    def __init__(self, functions, global_variables):
        self.global_variables = global_variables
        self.functions = functions

        for v in self.global_variables:
            v.address = '<global_%s>' % v.name

    def emit(self):
        # --- HEADER ---
        precode = []
        precode.append(['<CODE>', 'ARB', '<STACK/>'])
        precode.append(['<RB=...>', 'ADD', 0, '<STACK/>', 'RB[0]'])

        # Call main
        node_call = NodeCall('main', [])
        precode.extend(node_call.emit([]))

        # Stop
        precode.append(['<STOP>', 'STOP'])
        
        # --- CODE ---
        for node_fcn in self.functions:
            precode.extend(node_fcn.emit(self.global_variables))

        # Stop again. Unnecessary actually: Execution should not get here.
        # ... but it's a convenient way to label the end of the code-section
        precode.append(['</CODE>', 'STOP'])

        # --- DATA SECTION ---
        # global array variables can be large, but if they are uninitialized
        # we can be strategic and put them last in thie data-section. They
        # will then be rationalized away from the code

        # Sort in order:
        global_variables = sorted(self.global_variables, key=lambda v: 2 * int(v.array_size and v.array_size > 0) + int(v.array_init is None))

        # Emit all scalars and pointers
        for v in global_variables:
            if v.array_size and v.array_size > 0:
                precode.append(['<global_%s>' % v.name, '<global_%s_data>' % v.name])
                if v.array_init is not None and len(v.array_init) > v.array_size:
                    raise SyntaxError('Array cannot hold all that')
            else:
                precode.append(['<global_%s>' % v.name, 0])
        
        # Emit all array buffers
        for v in global_variables:
            if v.array_size and v.array_size > 0:
                if v.array_init is not None and len(v.array_init) <= v.array_size:
                    precode.append(['<global_%s_data>' % v.name, *(v.array_init + [0] * (v.array_size - len(v.array_init)))])
                elif v.array_init is None:
                    precode.append(['<global_%s_data>' % v.name, *([0] * v.array_size)])
                else:
                    raise SyntaxError('Array cannot hold all that')
        
        # That was all code and global memory. "Stack-memory" from here on.
        precode.append(['<STACK/>', 0])
        return precode


class NodeFunction:
    """Top-node of the program

    Holds functions and global variables.
    """
    def __init__(self, fname, parameters, block):
        self.fname = fname
        self.parameters = parameters
        self.block = block

        # Local variables from function body
        self.stack_size = self.block.setup_variables_on_stack(0) + len(self.parameters)

        # Parameters (treated like local variables)
        rbo = -self.stack_size
        for param in self.parameters:
            param.address = rbo
            rbo += 1
    
    def emit(self, global_variables):
        precode = []
        
        # Allocate memory on stack for return address, parameters and local variables
        #   (RB[0]-address: +1, return address: +1)
        rbo_step = 2 + self.stack_size
        precode.append(['<FCN_%s>' % self.fname, 'ARB', rbo_step])
        precode.append(['<RB_new=RB_old+%d>' % rbo_step, 'ADD', 'RB[%d]' % -rbo_step, rbo_step, 'RB[0]'])
        
        # Emit code body
        precode.extend(self.block.emit(global_variables + self.parameters, return_location='</FCN_%s>' % self.fname))
        # Restore stack (parameters and return address are also consumed)
        precode.append(['</FCN_%s>' % self.fname, 'ARB', -rbo_step])
        # Jump to designated return location
        precode.append([None, 'JZ', 0, 'RB[1]'])

        return precode


class NodeScope:
    def __init__(self, children, local_variables):
        self.local_variables = local_variables
        self.subscopes = [c for c in children if isinstance(c, NodeScope)]

    def setup_variables_on_stack(self, stack_size):
        for variable in self.local_variables:
            stack_size += 1 + int(variable.array_size) if variable.array_size else 1
            variable.address = -stack_size
        max_stack_size = stack_size
        for subscope in self.subscopes:
            max_stack_size = max(max_stack_size, subscope.setup_variables_on_stack(stack_size))
        return max_stack_size
    
    def emit(self, variables):
        existing_variables = [v.name for v in variables]

        # Setup all local array variables
        precode = []
        for v in self.local_variables:
            if v.name in existing_variables:
                raise SyntaxError('Variable name-collision for \'%s\'' % v.name)
            if v.array_size and v.array_size > 0:
                precode.append(['<int %s[]>' % v.name, 'ADD', 'RB[0]', v.address + 1, 'RB[%d]' % v.address])
            existing_variables.append(v.name)
        return precode


class NodeBlock(NodeScope):
    def __init__(self, statements, local_variables):
        super(NodeBlock, self).__init__(statements, local_variables)
        self.statements = statements

    def emit(self, variables, **kwargs):
        # Emit code for this scope's local variables
        precode = super(NodeBlock, self).emit(variables)
        
        # Delegate to statements
        for statement in self.statements:
            precode.extend(statement.emit(variables + self.local_variables, **kwargs))
        return precode


class NodeIfElse(NodeScope):
    _num = 0
    def __init__(self, condition, iftrue, iffalse=None):
        super(NodeIfElse, self).__init__([iftrue, iffalse], [])
        NodeIfElse._num += 1
        self.tag = 'IF_%d' % NodeIfElse._num
        self.condition = condition
        self.iftrue = iftrue
        self.iffalse = iffalse
        
    def emit(self, variables, **kwargs):
        # Emit code for this scope's local variables (shouldn't be any)
        precode = super(NodeIfElse, self).emit(variables)
        
        # Check condition
        precode.extend(self.condition.emit(variables, 1))

        # Jump around
        if self.iffalse is not None:
            precode.append(['<%s>' % self.tag, 'JZ', 'RB[1]', '<!%s>' % self.tag])
            precode.extend(self.iftrue.emit(variables, **kwargs))
            precode.append([None, 'JZ', 0, '</%s>' % self.tag])
            precode.append(['<!%s>' % self.tag, 'ARB', 0])
            precode.extend(self.iffalse.emit(variables, **kwargs))
            precode.append(['</%s>' % self.tag, 'ARB', 0])
        else:
            precode.append(['<%s>' % self.tag, 'JZ', 'RB[1]', '</%s>' % self.tag])
            precode.extend(self.iftrue.emit(variables, **kwargs))
            precode.append(['</%s>' % self.tag, 'ARB', 0])
        return precode


class NodeGoto:
    def __init__(self, location):
        self.location = location
    
    def emit(self, local_variables, **kwargs):
        return [['<GOTO>', 'JZ', 0, kwargs[self.location]]]


class NodeFor(NodeScope):
    _num = 0
    def __init__(self, condition, block, statement_1st=None, statement_every=None):
        super(NodeFor, self).__init__([block], [])
        NodeFor._num += 1
        self.tag = 'FOR_%d' % NodeFor._num
        self.condition = condition
        self.block = block
        self.statement_1st = statement_1st
        self.statement_every = statement_every
        
    def emit(self, variables, **kwargs):
        # Emit code for this scope's local variables (shouldn't be any)
        precode = super(NodeFor, self).emit(variables)

        # 1st
        if self.statement_1st is not None:
            precode.extend(self.statement_1st.emit(variables))
        
        # Check condition
        precode.append(['<%s>' % self.tag, 'ARB', 0])
        if self.condition is not None:
            precode.extend(self.condition.emit(variables, 1))
            precode.append([None, 'JZ', 'RB[1]', '<//%s>' % self.tag])

        
        # Block
        kwargs['break_location'] ='<//%s>' % self.tag
        kwargs['continue_location'] ='</%s>' % self.tag
        precode.extend(self.block.emit(variables, **kwargs))

        # Post-iteration statement
        precode.append(['</%s>' % self.tag, 'ARB', 0])
        if self.statement_every is not None:
            precode.extend(self.statement_every.emit(variables))
        precode.append([None, 'JZ', 0, '<%s>' % self.tag])
        
        # Finish
        precode.append(['<//%s>' % self.tag, 'ARB', 0])

        return precode


class NodeAssignment:
    def __init__(self, target, expression):
        self.target = target
        self.expression = expression
        # Assignment expression: trim last @-operand
        # (which must exist for a valid assignment)
        if self.target.postfix[-1] != '@':
            raise SyntaxError('Cannot assign to %s' % self.target.text)
        self.target.postfix = self.target.postfix[:-1]
    
    def emit(self, variables, **kwargs):
        precode = []
        precode.extend(self.target.emit(variables, 1))
        precode.extend(self.expression.emit(variables, 2))

        deref_tag = NodeExpression.unique_deref_tag()
        precode.append(['<%s>' % deref_tag, 'ADD', 0, 'RB[1]', '[</%s> + 3]' % deref_tag])
        precode.append(['</%s>' % deref_tag, 'ADD', 0, 'RB[2]', '[0]'])
        return precode


class NodeVariable:
    def __init__(self, name, array_size=None, is_global=False, array_init=None):
        self.name = name
        self.address = None
        self.array_size = array_size
        self.array_init = array_init
        self.is_global = is_global


class NodeCall:
    _num = 0
    def __init__(self, fname, parameters):
        NodeCall._num += 1
        self.call_id = 'CALL_%d' % NodeCall._num
        self.fname = fname
        self.parameters = parameters
        
    def emit(self, variables, **kwargs):
        precode = []

        # Special builtins (print() and scan())
        if self.fname == 'print':
            if len(self.parameters) != 1:
                raise SyntaxError('print() takes exactly 1 parameter')
            precode.extend(self.parameters[0].emit(variables, 1))
            precode.append(['<PRINT/>', 'OUT', 'RB[1]'])
            return precode
        elif self.fname == 'scan':
            if len(self.parameters) != 1:
                raise SyntaxError('scan() takes exactly 1 parameter')
            scan_call_id = 'SCAN_' + self.call_id[5:]
            precode.extend(self.parameters[0].emit(variables, 1))
            precode.append(['<%s>' % scan_call_id, 'ADD', 0, 'RB[1]', '[</%s> + 1]' % scan_call_id])
            precode.append(['</%s>' % scan_call_id, 'IN', '[0]'])
            return precode

        # Put return address on stack: 3 positions after the JZ-instruction below
        precode.append(['<%s>' % self.call_id, 'ADD', '</%s>' % self.call_id, 3, 'RB[1]'])

        # Put parameters (evaluate expressions as necessary) in RB[2], RB[3], ...
        for rbo, param in enumerate(self.parameters):
            precode.extend(param.emit(variables, rbo + 2))
        
        # Jump to designated location
        precode.append(['</%s>' % self.call_id, 'JZ', 0, '<FCN_%s>' % self.fname])

        return precode


class NodeExpression:
    _deref_num = 0
    def unique_deref_tag():
        NodeExpression._deref_num += 1
        return '@%d' % NodeExpression._deref_num

    def _infix_to_postfix(text):
        stk = deque()
        postfix = []
        precedence = {'==': 1, '<': 1, '>': 1, '<=': 1, '>=': 1, '+': 2, '-': 2, '*': 3, 'u+': 4, 'u-': 4, '&': 9, '@': 9}
        try:
            i = 0
            # '+' and '-' can sometimes be unary operators
            # use a state variable to determine when
            unary_op = True
            while i < len(text):
                if text[i].isspace():
                    i += 1
                elif text[i].isalnum() or text[i] == '_':
                    j = 1
                    while i+j < len(text) and (text[i+j].isalnum() or text[i+j] == '_'):
                        j += 1
                    
                    symbol = text[i:(i+j)]
                    postfix.append(symbol)
                    if is_valid_name(symbol):
                        postfix.append('@')  # "At"-operator
                    i += j
                    unary_op = False
                elif text[i] == '&':
                    stk.append('&')
                    i += 1
                    unary_op = True
                elif text[i] == '[':
                    stk.append('[')
                    i += 1
                    unary_op = True
                elif text[i] == '(':
                    stk.append('(')
                    i += 1
                    unary_op = True
                elif text[i] == ')':
                    while len(stk) > 0 and stk[-1] != '(':
                        postfix.append(stk.pop())
                    stk.pop() # Discard '('
                    i += 1
                    unary_op = False
                elif text[i] == ']':
                    while len(stk) > 0 and stk[-1] != '[':
                        postfix.append(stk.pop())
                    stk.pop()  # Discard '['
                    postfix.append('+') # Dereference
                    postfix.append('@')  # "At"-operator
                    i += 1
                    unary_op = False
                else:
                    if i + 1 < len(text) and text[i:(i+2)] in precedence:
                        symbol = text[i:(i+2)]
                        i += 2
                    else:
                        symbol = text[i]
                        i += 1
                    if symbol in ['+', '-'] and unary_op:
                        # Intercept unary operations "-a", "+a", ...
                        symbol = 'u' + symbol
                    unary_op = True
                    while len(stk) > 0 and stk[-1] != '(' and stk[-1] != '[' and precedence[symbol] <= precedence[stk[-1]]:
                        if stk[-1] == '&' and postfix[-1] == '@':
                            postfix.pop()
                            stk.pop()
                        else:
                            postfix.append(stk.pop())
                    stk.append(symbol)
            while len(stk) > 0:
                if stk[-1] == '&' and postfix[-1] == '@':
                    postfix.pop()
                    stk.pop()
                else:
                    postfix.append(stk.pop())
        except IndexError:
            raise SyntaxError('Failed to parse: \'%s\'' % text)
        
        return postfix

    def __init__(self, text):
        self.text = text
        self.postfix = NodeExpression._infix_to_postfix(text)
    
    def emit(self, variables, rbo):
        precode = []
        rbo0 = rbo
        for item in self.postfix:
            if item == '==':  # Compare 2 top items in stack
                assert rbo > rbo0
                rbo -= 1
                precode.append(['<x==y>', 'EQ', 'RB[%d]' % (rbo - 1), 'RB[%d]' % rbo, 'RB[%d]' % (rbo - 1)])
            elif item == '<':  # ...
                assert rbo > rbo0
                rbo -= 1
                precode.append(['<x<y>', 'LT', 'RB[%d]' % (rbo - 1), 'RB[%d]' % rbo, 'RB[%d]' % (rbo - 1)])
            elif item == '>':  # ...
                assert rbo > rbo0
                rbo -= 1
                precode.append(['<x>y>', 'LT', 'RB[%d]' % rbo, 'RB[%d]' % (rbo - 1), 'RB[%d]' % (rbo - 1)])
            elif item == '<=':  # ...
                assert rbo > rbo0
                rbo -= 1
                precode.append(['<x<=y>', 'LT', 'RB[%d]' % rbo, 'RB[%d]' % (rbo - 1), 'RB[%d]' % (rbo - 1)])
                precode.append(['<x<=y>', 'MUL', -1, 'RB[%d]' % (rbo - 1), 'RB[%d]' % (rbo - 1)])
                precode.append(['<x<=y>', 'ADD',  1, 'RB[%d]' % (rbo - 1), 'RB[%d]' % (rbo - 1)])
            elif item == '>=':  # ...
                assert rbo > rbo0
                rbo -= 1
                precode.append(['<x<=y>', 'LT', 'RB[%d]' % (rbo - 1), 'RB[%d]' % rbo, 'RB[%d]' % (rbo - 1)])
                precode.append(['<x<=y>', 'MUL', -1, 'RB[%d]' % (rbo - 1), 'RB[%d]' % (rbo - 1)])
                precode.append(['<x<=y>', 'ADD',  1, 'RB[%d]' % (rbo - 1), 'RB[%d]' % (rbo - 1)])
            elif item == '+':  # Add 2 top items in stack
                assert rbo > rbo0
                rbo -= 1
                precode.append(['<x+y>', 'ADD', 'RB[%d]' % (rbo - 1), 'RB[%d]' % rbo, 'RB[%d]' % (rbo - 1)])
            elif item == '-':  # Subtract 2 top items in stack
                assert rbo > rbo0
                rbo -= 1
                precode.append([None, 'MUL', -1, 'RB[%d]' % rbo, 'RB[%d]' % rbo])
                precode.append(['<x-y>', 'ADD', 'RB[%d]' % (rbo - 1), 'RB[%d]' % rbo, 'RB[%d]' % (rbo - 1)])
            elif item == 'u+': # no-invert. do nothing
                pass
            elif item == 'u-': # Invert sign
                precode.append(['<-x>', 'MUL', -1, 'RB[%d]' % (rbo - 1), 'RB[%d]' % (rbo - 1)])
            elif item == '*':  # Multiplicate 2 top items in stack
                assert rbo > rbo0
                rbo -= 1
                precode.append(['<x*y>', 'MUL', 'RB[%d]' % (rbo - 1), 'RB[%d]' % rbo, 'RB[%d]' % (rbo - 1)])
            elif item == '@':  # Dereference top item in stack
                deref_tag = NodeExpression.unique_deref_tag()
                precode.append(['<%s>' % deref_tag, 'ADD', 0, 'RB[%d]' % (rbo - 1), '[</%s> + 2]' % deref_tag])
                precode.append(['</%s>' % deref_tag, 'ADD', 0, '[0]', 'RB[%d]' % (rbo - 1)])
            elif item.isnumeric():
                precode.append(['<%s/>' % item, 'ADD', 0, int(item), 'RB[%d]' % rbo])
                rbo += 1
            else:
                # Must be variable (or?)
                for v in variables[::-1]:
                    if v.name == item:
                        break
                else:
                    raise SyntaxError('Unexpected \'%s\' in expression \'%s\'' % (item, self.text))
                
                if v.is_global:
                    precode.append(['<@global/>', 'ADD', 0, v.address, 'RB[%d]' % rbo])
                    #precode.append([None, 'ADD', 'RB[0]', v.address, 'RB[%d]' % rbo])
                    rbo += 1
                else:
                    precode.append(['<@local/>', 'ADD', 'RB[0]', v.address, 'RB[%d]' % rbo])
                    rbo += 1
        return precode


def is_valid_name(name):
    if not (name[0].isalpha() or name[0] == '_'):
        return False
    if not all([c.isalnum() or c == '_' for c in name]):
        return False
    return True


def is_empty_or_comment(text):
    return text.strip() == '' or text.lstrip().startswith('//')


def strip_ws_and_comments(text):
    comment = text.find('//')
    return text[:comment].strip() if comment >= 0 else text.strip()


def get_variable(text):
    text = text.strip()

    # Array type?
    left_bracket = text.rfind('[')
    right_bracket = text.rfind(']')
    if 0 < left_bracket and left_bracket < right_bracket:
        bracket_expression = text[(left_bracket + 1):right_bracket].strip()
        vname = text[:left_bracket].strip()
        array_type = True
    elif 0 < left_bracket and not left_bracket < right_bracket:
        raise SyntaxError('Missing \']\' after \'[\'')
    else:
        bracket_expression = ''
        vname = text.strip()
        array_type = False
    
    # Name?
    if not is_valid_name(vname):
        raise SyntaxError('Invalid variable-name \'%s\'' % vname)
    
    return vname, array_type, bracket_expression


def parse_statement(text, code, row):
    text = text.strip()
    if text == 'continue':
        return NodeGoto('continue_location')
    elif text == 'break':
        return NodeGoto('break_location')
    elif text == 'return':
        return NodeGoto('return_location')
    elif 0 < text.find('(') and (text.find('=') == -1 or text.find('(') < text.find('=')) and (text.find('[') == -1 or text.find('(') < text.find('[')):
        left_paren = text.find('(')
        right_paren = text.rfind(')')
        if not (0 < left_paren and left_paren < right_paren):
            raise SyntaxError('Parenthesis \'()\' mis-match', ('', row + 1, 0, code[row]))
        if not (is_valid_name(text[:left_paren]) and is_empty_or_comment(text[(right_paren + 1):])):
            raise SyntaxError('Failed to parse function call', ('', row + 1, 0, code[row]))
        fname = text[:left_paren]
        args = [a.strip() for a in text[(left_paren + 1):right_paren].split(',')]
        return NodeCall(fname, [NodeExpression(a) for a in args])
    elif 0 < text.find('+='):
        target = NodeExpression(text[:text.find('+=')].strip())
        expression = NodeExpression(target.text + '+(' + text[(text.find('+=') + 2):].strip() + ')')
        return NodeAssignment(target, expression)
    elif 0 < text.find('-='):
        target = NodeExpression(text[:text.find('-=')].strip())
        expression = NodeExpression(target.text + '-(' + text[(text.find('-=') + 2):].strip() + ')')
        return NodeAssignment(target, expression)
    elif 0 < text.find('*='):
        target = NodeExpression(text[:text.find('*=')].strip())
        expression = NodeExpression(target.text + '*(' + text[(text.find('*=') + 2):].strip() + ')')
        return NodeAssignment(target, expression)
    elif 0 < text.find('='):
        target = NodeExpression(text[:text.find('=')].strip())
        expression = NodeExpression(text[(text.find('=') + 1):].strip())
        return NodeAssignment(target, expression)


def read_variable_definition(code, row, is_global=False):
    # -------------------------------------------
    # Parse a variable definition
    #
    # primitive:  int a
    #     array:  int b[10]
    # -------------------------------------------

    semicolon = code[row].find(';')
    if semicolon < 0 or not is_empty_or_comment(code[row][(semicolon + 1):]):
        raise SyntaxError('Invalid variable-definition', ('', row + 1, 0, code[row]))
    line = code[row][:semicolon].strip()
    if not line.startswith('int') or len(line) < 4  or line[4].isspace():
        raise SyntaxError('Only int-types allowed. (not \'%s\')' % line.split(' ')[0])
    vname, array_type, bracket_expression = get_variable(line[3:])

    if array_type:
        # Parse
        right_bracket = code[row].find(']')
        equals = code[row].find('=')
        left_curly = code[row].find('{')
        right_curly = code[row].find('}')
        left_cite = code[row].find('"')
        right_cite = code[row].rfind('"')

        # Extract initialization
        if right_bracket < equals and equals < left_cite and left_cite < right_cite and right_cite < semicolon:
            array_init = [ord(x) for x in code[row][(left_cite + 1):right_cite]] + [0]
        elif right_bracket < equals and equals < left_curly and left_curly < right_curly and right_curly < semicolon:
            array_init = [int(x) for x in code[row][(left_curly + 1):right_curly].split(',')]
        elif equals < 0 and left_curly < 0 and right_curly < 0:
            array_init = None
        else:
            raise SyntaxError('Illegal array-initialization', ('', row + 1, 0, code[row]))
        
        # Array size
        if bracket_expression == '' and array_init is not None:
            array_size = len(array_init)
        elif bracket_expression.isnumeric():
            array_size = int(bracket_expression)
            if array_size <= 0:
                raise SyntaxError('Array-size must be positive, not \'%d\'' % array_size, ('', row + 1, 0, code[row]))
        else:
            raise SyntaxError('Invalid array-size (expected int-constant): \'%s\'' % bracket_expression, ('', row + 1, 0, code[row]))

        return NodeVariable(vname, array_size, is_global, array_init), row + 1
    else:
        return NodeVariable(vname, None, is_global, None), row + 1


def read_forloop(code, row):
    # -------------------------------------------
    # Read for-loop block
    #
    # row --> for (assignment; condition; assignment) {
    #            ...
    #         }
    #         else <...>
    # -------------------------------------------
    forloop_line = strip_ws_and_comments(code[row])
    left_paren = forloop_line.find('(')
    right_paren = forloop_line.rfind(')')
    left_curly = forloop_line.rfind('{')
    semicolon1 = forloop_line.find(';')
    semicolon2 = forloop_line.rfind(';')
    if not (forloop_line.startswith('for') and 0 < left_paren and left_paren < semicolon1 and semicolon1 < semicolon2 and semicolon2 < right_paren and right_paren < left_curly):
        raise SyntaxError('Expected \'for (...; ...; ...) {\'', ('', row + 1, 0, code[row]))
    
    statement1_text = forloop_line[(left_paren+1):semicolon1].strip()
    condition_text = forloop_line[(semicolon1+1):semicolon2].strip()
    statement2_text = forloop_line[(semicolon2+1):right_paren].strip()
    should_be_empty_1 = forloop_line[(right_paren+1):left_curly].strip()
    should_be_empty_2 = forloop_line[(left_curly+1):].strip()

    if not (should_be_empty_1 == ''):
        raise SyntaxError('Unexpected \'%s\' in if-else-statement' % should_be_empty_1, ('', row + 1, 0, code[row]))
    if not (should_be_empty_2 == ''):
        raise SyntaxError('Unexpected \'%s\' in if-else-statement' % should_be_empty_2, ('', row + 1, 0, code[row]))
    
    condition = NodeExpression(condition_text) if condition_text != '' else None

    statements = []
    for statement_text in [statement1_text, statement2_text]:
        if statement_text == '':
            statements.append(None)
        else:
            statements.append(parse_statement(statement_text, code, row))
    
    block, row = read_block(code, row + 1)
    
    return NodeFor(condition, block, statements[0], statements[1]), row


def read_ifelse(code, row):
    # -------------------------------------------
    # Read if-else block
    #
    # row --> if (condition) {
    #            ...
    #         }
    #         else <...>
    # -------------------------------------------
    if_line = code[row].strip()
    left_paren = if_line.find('(')
    right_paren = if_line.rfind(')')
    left_curly = if_line.rfind('{')
    if not ((if_line.startswith('if') or if_line.startswith('else if')) and 0 < left_paren and left_paren < right_paren and right_paren < left_curly):
        raise SyntaxError('Expected \'if (...) {\' or \'else if (...) {\'', ('', row + 1, 0, code[row]))
    
    condition = NodeExpression(if_line[(left_paren+1):right_paren])
    should_be_empty_1 = if_line[(right_paren+1):left_curly].strip()
    should_be_empty_2 = if_line[(left_curly+1):].strip()

    if not (should_be_empty_1 == ''):
        raise SyntaxError('Unexpected \'%s\' in if-else-statement' % should_be_empty_1, ('', row + 1, 0, code[row]))
    if not is_empty_or_comment(should_be_empty_2):
        raise SyntaxError('Unexpected \'%s\' in if-else-statement' % should_be_empty_2, ('', row + 1, 0, code[row]))

    if_block, row = read_block(code, row + 1)

    else_line = code[row].strip()
    if else_line.startswith('else if'):
        else_block, row = read_ifelse(code, row)
    elif else_line.startswith('else'):
        left_curly = else_line.rfind('{')
        should_be_empty_1 = else_line[4:left_curly].strip()
        should_be_empty_2 = else_line[(left_curly+1):].strip()
        if not (should_be_empty_1 == ''):
            raise SyntaxError('Unexpected \'%s\' in if-else-statement' % should_be_empty_1, ('', row + 1, 0, code[row]))
        if not is_empty_or_comment(should_be_empty_2):
            raise SyntaxError('Unexpected \'%s\' in if-else-statement' % should_be_empty_2, ('', row + 1, 0, code[row]))
        else_block, row = read_block(code, row + 1)
    else:
        else_block = None
    return NodeIfElse(condition, if_block, else_block), row


def read_block(code, row):
    # -------------------------------------------
    # Read a block in its entirety
    #
    #         function / if / while / ...  {
    # row -->   .
    #           .
    #           .
    #         }
    # -------------------------------------------
    local_variables = []
    statements = []
    while(row < len(code)):
        # Strip comments once and for all on this line
        line = strip_ws_and_comments(code[row])

        if is_empty_or_comment(line):
            row += 1
        elif line.startswith('int') and line[3].isspace():
            variable, row = read_variable_definition(code, row)
            local_variables.append(variable)
        elif line.startswith('if') and line[2:].lstrip().startswith('('):
            ifelse, row = read_ifelse(code, row)
            statements.append(ifelse)
        elif line.startswith('for') and line[3:].lstrip().startswith('('):
            forloop, row = read_forloop(code, row)
            statements.append(forloop)
        elif line.startswith('while') and line[5:].lstrip().startswith('('):
            warnings.warn('NYI')
            row = read_block(code, row+1)
        elif line == '}':
            row += 1
            break
        elif line.endswith(';'):
            statements.append(parse_statement(line[:-1], code, row))
            row += 1
        else:
            raise SyntaxError('Invalid expression', ('', row + 1, 0, code[row]))
    else:
        raise SyntaxError('Unexpected EOF', ('', row + 1, 0, 'EOF'))
    
    return NodeBlock(statements, local_variables), row


def read_function(code, row):
    # -------------------------------------------
    # Read function
    #
    # row --> void foo(int a, int b, int* result) {
    #            ...
    #         }
    # -------------------------------------------

    # Verify function structure
    left_paren = code[row].find('(')
    right_paren = code[row].rfind(')')
    left_curly = code[row].rfind('{')
    if not (code[row].startswith('void ') and 0 < left_paren and left_paren < right_paren and right_paren < left_curly):
        raise SyntaxError('Expected \'void <function-name>(...) {\'', ('', row + 1, 0, code[row]))
    
    # Read function name and definition
    fname = code[row][5:left_paren].strip()
    param_list = code[row][(left_paren+1):right_paren]
    should_be_empty_1 = code[row][(right_paren+1):left_curly].strip()
    should_be_empty_2 = code[row][(left_curly+1):].strip()
    if not is_valid_name(fname):
        raise SyntaxError('Invalid function-name \'%s\'' % fname, ('', row + 1, 0, code[row]))
    if not (should_be_empty_1 == ''):
        raise SyntaxError('Unexpected \'%s\' in function definition' % should_be_empty_1, ('', row + 1, 0, code[row]))
    if not is_empty_or_comment(should_be_empty_2):
        raise SyntaxError('Unexpected \'%s\' in function definition' % should_be_empty_2, ('', row + 1, 0, code[row]))

    # Read parameters
    param_defs = [param_def.strip() for param_def in param_list.split(',')]
    if param_defs[0] == '':
        param_defs.pop(0)
    if not all([param_def.startswith('int') and param_def[3].isspace() for param_def in param_defs]):
        raise SyntaxError('Only int-type is allowed for parameters', ('', row + 1, 0, code[row]))
    variables = [get_variable(param_def[3:]) for param_def in param_defs]
    if any([v[1] for v in variables]):
        raise SyntaxError('No array-types are allowed for parameters', ('', row + 1, 0, code[row]))
    params = [NodeVariable(v[0]) for v in variables]
    
    # Read function body
    block, row = read_block(code, row + 1)
    
    return NodeFunction(fname, params, block), row


def read_file(input_file):
    functions = []
    global_variables = []

    with open(input_file) as f:
        code = f.read().splitlines(keepends=False)
    
    try:
        row = 0
        while row < len(code):
            # Strip comments once and for all on this line
            line = strip_ws_and_comments(code[row])

            if is_empty_or_comment(line):
                row += 1
            elif line.startswith('int') and line[3].isspace():
                variable, row = read_variable_definition(code, row, is_global=True)
                global_variables.append(variable)
            elif line.startswith('void') and line[4].isspace():
                node_fcn, row = read_function(code, row)
                functions.append(node_fcn)
    except SyntaxError as e:
        print('\nERROR: {}, in {}:'.format(e.msg, input_file))
        print('\n{}: {}'.format(e.lineno, e.text))
        sys.exit(1)
    return functions, global_variables


def assemble_precode_to_intcode(precode):
    intcode = []
    label_lookup = dict()
    labels = dict()

    opcodes = {
        'ADD': 1,
        'MUL': 2,
        'IN': 3,
        'OUT': 4,
        'JNZ': 5,
        'JZ': 6,
        'LT': 7,
        'EQ': 8,
        'ARB': 9,
        'STOP': 99
    }

    # PASS 1
    for line in precode:
        if line[0] is not None:
            label_lookup[line[0]] = len(intcode)
            labels[len(intcode)] = line[0]
        
        if line[1] not in opcodes:
            intcode.extend(line[1:])
            continue

        # Parameter modes
        assert line[1] in opcodes, 'Invalid op: %s' % line[1]
        intcode.append(opcodes[line[1]])
        parameter_mode = ''
        args = []
        for c in line[2:]:
            if isinstance(c, int):
                parameter_mode = '1' + parameter_mode  # Direct mode
                args.append(c)
            elif isinstance(c, str) and c.startswith('<'):
                assert c.endswith('>'), 'Invalid arg: %s' % c
                parameter_mode = '1' + parameter_mode  # Direct mode
                args.append(c)
            elif isinstance(c, str) and c.startswith('['):
                assert c.endswith(']'), 'Invalid arg: %s' % c
                parameter_mode = '0' + parameter_mode    # Relative mode
                args.append(c[1:-1])
            elif isinstance(c, str) and c.startswith('RB['):
                assert c.endswith(']'), 'Invalid arg: %s' % c
                parameter_mode = '2' + parameter_mode    # Relative mode
                args.append(c[3:-1])
            else:
                raise ValueError('%s not expected here' % c)
        intcode[-1] += 100*int(parameter_mode) if parameter_mode != '' else 0
        intcode.extend(args)
    
    # PASS 2
    for p in range(len(intcode)):
        c = intcode[p]
        if isinstance(c, str):
            if len(c.split()) == 3 and c.split()[1] in ['+', '-']:
                offset = int(c.split()[2]) if c.split()[1] == '+' else -int(c.split()[2])
                c = c.split()[0]
                intcode[p] = label_lookup.get(c, c) + offset
            else:
                intcode[p] = label_lookup.get(c, c)
        try:
            intcode[p] = int(intcode[p])
        except ValueError:
            print('\nERROR: Symbol \'{}\' could not be resolved.'.format(intcode[p]))
            sys.exit(1)
    
    return intcode, label_lookup, labels


def functions_to_link(functions, global_variables):
    fcn_dict = dict()
    for f in functions:
        assert f.fname not in fcn_dict, 'Duplicate function: %s' % f.fname
        fcn_dict[f.fname] = f
    
    glb_dict = dict()
    for g in global_variables:
        assert g.name not in glb_dict, 'Duplicate global variable: %s' % g.name
        glb_dict[g.name] = g

    assert 'main' in fcn_dict, 'Missing main-function'

    fcn_to_link = set()
    glb_to_link = set()
    bfsQ = deque()
    bfsQ.append(fcn_dict['main'])
    while len(bfsQ) > 0:
        node = bfsQ.popleft()
        if isinstance(node, NodeFunction):
            if node.fname not in fcn_to_link:
                fcn_to_link.add(node.fname)
                bfsQ.append(node.block)
        elif isinstance(node, NodeCall):
            if node.fname in fcn_dict:
                bfsQ.append(fcn_dict[node.fname])
            bfsQ.extend(node.parameters)
        elif isinstance(node, NodeBlock):
            bfsQ.extend(node.statements)
        elif isinstance(node, NodeIfElse):
            bfsQ.extend([node.condition, node.iftrue, node.iffalse])
        elif isinstance(node, NodeFor):
            bfsQ.extend([node.statement_1st, node.condition, node.statement_every, node.block])
        elif isinstance(node, NodeAssignment):
            bfsQ.extend([node.target, node.expression])
        elif isinstance(node, NodeExpression):
            for item in node.postfix:
                if item in glb_dict:
                    glb_to_link.add(item)
    return fcn_to_link, glb_to_link


def main():
    if len(sys.argv) == 1:
        sys.argv = ['intcode_cc.py', '-v', '-i', 'math.c', 'string.c', 'sort.c', 'test.c', '-o', 'test.txt']
        #sys.argv = ['intcode_cc.py', '-v', '-i', 'test_neg.c', '-o', 'test.txt']

    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', type=str, nargs='+', required=True)
    parser.add_argument('-o', '--output', type=str, default=None)
    parser.add_argument('-v', '--verbose', action='store_true')
    args = parser.parse_args()

    # Read code
    functions = []
    global_variables = []
    for input_file in args.input:
        _functions, _global_variables = read_file(input_file)
        functions.extend(_functions)
        global_variables.extend(_global_variables)
    
    # Prune unnecessary code and global variables
    fcn_to_link, glb_to_link = functions_to_link(functions, global_variables)
    if args.verbose:
        print('---------------- FUNCTIONS ----------------')
        for f in fcn_to_link:
            print('  void %s()' % f)
        print('---------------- GLOBAL VARIABLES ----------------')
        for g in glb_to_link:
            print('  int %s' % g)
    functions = [f for f in functions if f.fname in fcn_to_link]
    global_variables = [g for g in global_variables if g.name in glb_to_link]

    # Compile!
    try:
        program = NodeProgram(functions, global_variables)
        precode = program.emit()
    except SyntaxError as e:
        print('\nERROR: {}'.format(e.msg, input_file))
        sys.exit(1)

    if args.verbose:
        # Print what we got
        print('---------------- PSEUDO INTCODE ----------------')
        for line in precode:
            if line[0] is not None:
                print(('%30s : ' % line[0]) + str(line[1:]))
            else:
                print(('%30s : ' % '') + str(line[1:]))

    # Link!
    intcode, label_lookup, _ = assemble_precode_to_intcode(precode)

    if args.verbose:
        print('---------------- LABELS ----------------')
        for label in label_lookup:
            l = label.strip()[1:-1]
            while l[0] == '/':
                l = l[1:]
            if l[-1] == '/':
                l = l[:-1]
            try:
                int(l)
                continue
            except:
                pass
            if l.startswith('@') or l.startswith('RB=') or l.startswith('RB_new'):
                continue
            if l.startswith('IF_') or l.startswith('!IF_') or l.startswith('FOR_') or l.startswith('CALL_'):
                pass #continue
            if l.startswith('SCAN_') or l.startswith('PRINT'):
                continue
            if l in ['x+y', 'x-y', 'x*y', 'x<y', 'x>y', 'x<=y', 'x>=y', 'x==y', '-x']:
                continue
            print('%30s : %d' % (label.strip(), label_lookup[label]))

    # Prune trailing 0's
    last_nonzero = max(idx for idx, val in enumerate(intcode) if val != 0)
    intcode = intcode[:(last_nonzero+1)]

    if args.output is not None:
        with open(args.output, 'w') as f:
            f.write(','.join(['%d' % c for c in intcode]))
            f.write('\n')


if __name__ == '__main__':
    main()