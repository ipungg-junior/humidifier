from django.shortcuts import render, HttpResponse, redirect
from django.http import HttpResponseForbidden
from django.views.decorators.csrf import csrf_exempt
from django.utils.decorators import method_decorator
from django.views.decorators.cache import never_cache
from django.views import View
from django.contrib.auth.decorators import login_required
from .models import *
from django.contrib.auth import authenticate, login, logout
from .services.ESP32 import Esp32
from .services.DBManagement import ManagementDevice, ManagementAccount
from .forms import *
import json

@method_decorator(never_cache, name='dispatch')
class LandingPage(View):
    template = 'index.html'
    def get(self, req):
        
        return render(req, self.template, context={
            'title': 'Humidifier',
        })

@method_decorator(never_cache, name='dispatch')
class Account(View):
    template = ''
    context = ''

    def get(self, req):
        if (self.context=='login'):
            if (req.user.is_anonymous):    
                form = LoginForm()
                return render(req, 'login.html', 
                    context={
                        'form': form
                    })
            else:
                return redirect('/monitoring/')

        if (self.context=='register'):
            if (req.user.is_anonymous):    
                form = RegisterForm()
                return render(req, 'register.html', 
                    context={
                        'form': form
                    })
            else:
                return redirect('/monitoring/')

        if (self.context=='logout'):
            logout(req)
            return redirect('/')


    def post(self, req):
        if (self.context=='login'):
            user = authenticate(req, username=req.POST['email'], password=req.POST['password'])
            if user is not None:
                login(req, user)
                print(f'{user} berhasil login.')
                return redirect('/monitoring/')
            else:
                print('{user} : Invalid password')
                form = LoginForm() 
                return render(req, 'login.html', context={
                    'form': form
                })

        if (self.context=='register'):
            new_user = ManagementAccount.create_user(req)
            if new_user is not None:
                login(req, new_user)
                return redirect('/monitoring/')
            else:
                print('GAGAL MEMBUAT AKUN BARU !')
                form = RegisterForm() 
                return render(req, 'register.html', context={
                    'form': form
                })

        else:
            return HttpResponse(code=401)


@method_decorator(csrf_exempt, name='dispatch')
class EspView(View):

    context = ''

    @csrf_exempt
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


@method_decorator(never_cache, name='dispatch')
@method_decorator(csrf_exempt, name='dispatch')
class Monitoring(View):

    context = ''

    def get(self, req, deviceID=None):

        if (req.user.is_authenticated):
            if (self.context=='monitoring-list'):
                device_list = req.user.clientdevice_set.all()
                form = AddDeviceForm()
                response = render(req, 'monitoring.html', context={
                    'title': 'Monitoring',
                    'device_list' : device_list,
                    'form' : form
                })
                response['Cache-Control'] = 'no-cache'
                return response
                
            if (self.context=='monitoring-detail'):
                device = ManagementDevice.getDeviceInfo(req.user, deviceID)
                if device is None:
                    return HttpResponseForbidden()
                return render(req, 'device_detail.html', context={
                    'title' : f'Detail {device.deviceID}',
                    'device': device
                    })

        else:
            return redirect('/account/login/')


    def post(self, req):
        for key, value in req.session.items():
            print('{} => {}'.format(key, value))    
        return HttpResponse('yay', status=200)


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
                status = ManagementDevice.registerDeviceOnFirebase(deviceID=data['deviceID'])
                return HttpResponse(status=200)
        else:
            print('here')
            response_unauthorized = HttpResponse()
            response_unauthorized.status_code = 401
            response_unauthorized.content = "<h1> Access Denied </h1>"
            return response_unauthorized



@login_required(login_url='/account/login/', redirect_field_name='/monitoring/')
def deviceUsage(req, deviceID):
    status, all_session = ManagementDevice.showHistory(req.user, deviceID)
    if status:
        return render(req, 'device_usage.html', context={'all_session': all_session})
    else:
        return redirect('/monitoring/')
    


