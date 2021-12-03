import os
import requests
from datetime import datetime

# Getting session-cookie using Chrome:
# - Navigate to Advent of Code and login
# - Right-click > "Inspect" > "Application"-tab
# - "Storage" > "Session Storage" > "Cookies" > https://adventofcode.com
# - Copy&paste to session.txt

def _download_input(day, year):
    url = 'https://adventofcode.com/%d/day/%d/input' % (year, day)

    if not os.path.isfile('session.txt'):
        raise FileNotFoundError('Missing \'session.txt\' (read in source-code how to obtain)')
    with open('session.txt') as f:
        session = f.readline()

    try:
        print('Downloading...')
        read = requests.get(url, cookies={'session': session})
        data = [chunk for chunk in read.iter_content(chunk_size=512)]
        data = b''.join(data).decode()
        print('Done!')
        return data
    except requests.RequestException as e:
        print('Error :%s' % e)
        return None


def get_input(day, year):
    # Cache input to avoid unnecessary server-load
    filename = os.path.join('%4d' % year, 'input%02d.txt' % day)
    if os.path.isfile(filename):
        print('Reading local \'%s\'...' % filename)
        with open(filename) as f:
            data = f.read()
        print('Done!')
    else:
        print('No local file.')
        data = _download_input(day, year)
        if not data:
            return None
        print('Storing locally as \'%s\'...' % filename)
        with open(filename, 'w') as f:
            f.write(data)
        print('Done!')
    return data


if __name__ == '__main__':
    input = get_input(datetime.now().day, datetime.now().year)
    print('----- INPUT (input%02d.txt) -----' % datetime.now().day)
    print(input)
    print('-------------------------------')
