from django.urls import path
from .views import *
from django.conf import settings # to import static in deployment


urlpatterns = [
    path('', LandingPage.as_view()),                
    path('account/register/', Account.as_view(context='register')),
    path('account/login/', Account.as_view(context='login')),
    path('account/logout/', Account.as_view(context='logout')),    
    path('monitoring/', Monitoring.as_view(context='monitoring-list')),                        
    path('monitoring/<str:deviceID>/', Monitoring.as_view(context='monitoring-detail')),
    path('monitoring/<str:deviceID>/usage/', deviceUsage),
    
    # Service API esp32
    path('service/register/', EspView.as_view(context='register-device')),
    path('service/publish/', EspView.as_view(context='publish')),
    path('service/register-session/', EspView.as_view(context='register-session')),
    path('service/disconnect/', EspView.as_view(context='disconnect')),
    
    #root-access!! (looping delete object DeviceUsage table)
    #path('service/delete/device-table/', deleteDeviceTable)
]