import ast
import json


class JSONServices:

    @staticmethod
    def success_register(status, link):
        __RESPONSES = {}
        __RESPONSES['status'] = status
        __RESPONSES['link'] = link
        return json.dumps(__RESPONSES)


    @staticmethod
    def decode(req):
        data = (req).decode('ascii')
        return ast.literal_eval(data)