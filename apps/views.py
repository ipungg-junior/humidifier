from django.shortcuts import render, HttpResponse, redirect
from django.http import HttpResponseForbidden
from django.views.decorators.csrf import csrf_exempt
from django.utils.decorators import method_decorator
from django.views import View
from django.contrib.auth.decorators import login_required
from .models import *
from django.contrib.auth import authenticate, login, logout
from .services.ESP32 import Esp32
from .services.DBManagement import ManagementDevice, ManagementAccount
from .forms import *


class LandingPage(View):
    template = 'index.html'
    def get(self, req):
        print(req.user)
        
        return render(req, self.template, context={
            'title': 'Humidifier',
        })


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
                print(f'{new_user} berhasil login.')
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
        if (self.context=='register-device'):
            ret = Esp32.register(req)
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
        return HttpResponse(status=400)


class Monitoring(View):

    context = ''

    def get(self, req, deviceID=None):

        if (req.user.is_authenticated):
            if (self.context=='monitoring-list'):
                device_list = req.user.clientdevice_set.all()
                form = AddDeviceForm()
                return render(req, 'monitoring.html', context={
                    'title': 'Monitoring',
                    'device_list' : device_list,
                    'form' : form
                })
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
        pass




@login_required(login_url='/account/login/', redirect_field_name='/monitoring/')
def deviceUsage(req, deviceID):
    status, all_session = ManagementDevice.showHistory(req.user, deviceID)
    if status:
        return render(req, 'device_usage.html', context={'all_session': all_session})
    else:
        return redirect('/monitoring/')
    

@login_required(login_url='/account/login/', redirect_field_name='/monitoring/')
def deviceDetail(req, deviceID):
    device_list = req.user.clientdevice_set.all()
    return render(req, 'device_detail.html', context={
        'device_list' : device_list
    })


