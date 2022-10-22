from django.shortcuts import render, HttpResponse, redirect
from django.contrib.auth.decorators import login_required
from .services.DBManagement import ManagementDevice


@login_required(login_url='/account/login/', redirect_field_name='/monitoring/')
def deviceUsage(req, deviceID):
    status, all_session = ManagementDevice.showHistory(req.user, deviceID)
    if status:
        return render(req, 'device_usage.html', context={'all_session': all_session})
    else:
        return redirect('/monitoring/')
    


