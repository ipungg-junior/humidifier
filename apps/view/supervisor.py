from django.views import View
from django.views.decorators.cache import never_cache
from django.shortcuts import HttpResponse, render
from django.utils.decorators import method_decorator
from apps.services.DBManagement import ManagementDevice
import json

@method_decorator(never_cache, name='dispatch')
class Supervisor(View):
    context = ''

    def get(self, req):
        if (req.user.is_superuser):
            if (self.context=='log-all-device'):
                query_device = ManagementDevice.getAllDevice(req.user)
                return render(req, 'supervisor_log.html', context={'deviceList': query_device})
            if (self.context=='log-device'):
                pass
            if (self.context=='device-registrar'):
                return render(req, "supervisor_device_registrar.html")
        
        else:
            response_unauthorized = HttpResponse()
            response_unauthorized.status_code = 401
            response_unauthorized.content = "<h1> Access Denied </h1>"
            return response_unauthorized

    def post(self, req):
        if (req.user.is_superuser):
            if (self.context == 'log-all-device'):
                data_str = (req.body).decode('utf-8')
                data = json.loads(data_str)
                return HttpResponse("{'s':{data}}", status=200)

            else:
                data_str = (req.body).decode('utf-8')
                data = json.loads(data_str)
                status = ManagementDevice.device_registrar(deviceID=data['deviceID'])
                if status:
                    return HttpResponse(status=200)
                else:
                    return HttpResponse(status=401)
        else:
            print('here')
            response_unauthorized = HttpResponse()
            response_unauthorized.status_code = 401
            response_unauthorized.content = "<h1> Access Denied </h1>"
            return response_unauthorized