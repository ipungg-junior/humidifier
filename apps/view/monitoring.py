from django.views import View
from django.shortcuts import HttpResponse, redirect
from django.utils.decorators import method_decorator
from django.views.decorators.csrf import csrf_exempt
from django.http import HttpResponseForbidden
from apps.services.ResponsesWorker import DefaultResponse
from apps.forms import AddDeviceForm
from apps.services.DBManagement import ManagementDevice

@method_decorator(csrf_exempt, name='dispatch')
class Monitoring(View):

    context = ''
 
    def get(self, req, deviceID=None):

        if (req.user.is_authenticated):
            if (self.context=='monitoring-list'):
                device_list = req.user.clientdevice_set.all()
                form = AddDeviceForm()
                response = DefaultResponse.defaultResponse(req, 'monitoring.html', cache=False, context={
                    'title': 'Monitoring',
                    'device_list' : device_list,
                    'form' : form
                })
                return response
                
            if (self.context=='monitoring-detail'):
                device = ManagementDevice.getDeviceInfo(req.user, deviceID)
                if device is None:
                    return HttpResponseForbidden()
                response = DefaultResponse.defaultResponse(req, 'device_detail.html', cache=False, context={
                    'title' : f'Detail {device.deviceID}',
                    'device': device
                })
                return response

        else:
            return redirect('/account/login/')


    def post(self, req):
        for key, value in req.session.items():
            print('{} => {}'.format(key, value))    
        return HttpResponse('yay', status=200)