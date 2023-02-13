from django.views import View
from apps.services.ResponsesWorker import DefaultResponse
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
import json
from django.utils.decorators import method_decorator

@method_decorator(csrf_exempt, name='dispatch')
class LandingPage(View):
    template = 'index.html'
    context = ''
    def get(self, req): 

        if (self.context == "test-invoice"):
            response = DefaultResponse.defaultResponse(req, "invoice.html", context={
            'title': 'Humidifier',
            }, cache=False)
            return response

        response = DefaultResponse.defaultResponse(req, self.template, context={
            'title': 'Humidifier',
        }, cache=False)
        return response 

    def post(self, req):
        if (self.context == "create-payment"):
            x = (req.body.decode('utf-8'))
            data = json.loads(x)
            print("=================================================")
            print(data)
            print("=================================================")
            response = HttpResponse(status=200)
            return response 
        if (self.context == "payment-created"):
            x = (req.body.decode('utf-8'))
            data = json.loads(x)
            print("=================================================")
            print(data)
            print("=================================================")
            response = HttpResponse(status=200)
            return response 
        if (self.context == "accept-payment"):
            x = (req.body.decode('utf-8'))
            data = json.loads(x)
            print("=================================================")
            print(data)
            print("=================================================")
            response = HttpResponse(status=200)
            return response 
            