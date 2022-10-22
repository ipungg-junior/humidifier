from django.views import View
from apps.services.ResponsesWorker import DefaultResponse



class LandingPage(View):
    template = 'index.html'
    
    def get(self, req): 
        response = DefaultResponse.defaultResponse(req, self.template, context={
            'title': 'Humidifier',
        }, cache=False)
        return response