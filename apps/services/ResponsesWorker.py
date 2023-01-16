from django.shortcuts import render, HttpResponse, redirect

class DefaultResponse:

    @staticmethod
    def defaultResponse(request, template, context, cache=False):
        response = render(request, template, context=context)
        if (cache==False):
            response['Pragma'] = 'no-cache'
            response['Cache-Control'] = 'no-cache'
        return response