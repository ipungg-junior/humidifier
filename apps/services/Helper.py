import random

def randomNum(size, min=1, max=9):
    '''
        Generate random number with range 1-99 (default).
        return string type
    '''
    num = ''
    for i in range(size):
        num += str(random.randint(min,max))
    return num


def convertToList(data):
    result = list()
    data = str(data).split('&')
    for i in data:
        result.append(i.split('=')[1])

    return result