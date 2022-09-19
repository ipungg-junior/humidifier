from apps.services.DBManagement import ManagementDevice
from django.shortcuts import render, HttpResponse, redirect
from django.http import HttpResponseForbidden
from django.views.decorators.csrf import csrf_exempt
from django.utils.decorators import method_decorator
from django.views.decorators.cache import never_cache
from django.views import View
from django.contrib.auth.decorators import login_required


class Log(View):
    context = ''

    def get(self, req, deviceID=None):
        if ('log-all-device'):
            device_list = ManagementDevice.getAllDevice(auth_user=req.user)
            return render(req, 'supervisor_log.html', context={'list': device_list})
        if ('log-device'):
            pass
        