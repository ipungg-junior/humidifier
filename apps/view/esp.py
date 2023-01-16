from django.views.decorators.csrf import csrf_exempt
from django.utils.decorators import method_decorator
from django.views import View
from apps.services.ESP32 import Esp32

@method_decorator(csrf_exempt, name='dispatch')
class EspAPI(View):

    context = ''

    def post(self, req):
        if (self.context=='linking-device'):
            ret = Esp32.linking(req)
            return ret  
        if (self.context=='register-session'):
            ret = Esp32.registerSession(req)
            return ret
        if (self.context=='publish'):
            ret = Esp32.receive(req)
            return ret
        if (self.context=='disconnect'):
            ret = Esp32.disconnected(req)
            return ret

        

    def get(self, req):
        if (self.context=='register-machine-code'):
            ret = Esp32.registerMachineCode()
            return ret  