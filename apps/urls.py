from django.urls import path
from .views import deviceUsage
from django.conf import settings # to import static in deployment
from apps.view.landing import LandingPage
from apps.view.account import Account
from apps.view.esp import EspAPI
from apps.view.monitoring import Monitoring
from apps.view.supervisor import Supervisor

urlpatterns = [
    # WEB-BASED INTERFACE
    path('', LandingPage.as_view()),                
    path('account/register/', Account.as_view(context='register')),
    path('account/login/', Account.as_view(context='login')),
    path('account/logout/', Account.as_view(context='logout')),    
    path('monitoring/', Monitoring.as_view(context='monitoring-list')),                        
    path('monitoring/<str:deviceID>/', Monitoring.as_view(context='monitoring-detail')),
    path('monitoring/<str:deviceID>/usage/', deviceUsage),
    
    # Humidifier API
    path('service/register-machine-code/', EspAPI.as_view(context='register-machine-code')),
    path('service/linking/', EspAPI.as_view(context='linking-device')),
    path('service/publish/', EspAPI.as_view(context='publish')),
    path('service/register-session/', EspAPI.as_view(context='register-session')),
    path('service/disconnect/', EspAPI.as_view(context='disconnect')),
    
    #root-access!! (becareful)
    path('supervisor/device-registrar/', Supervisor.as_view(context='device-registrar')),
    path('supervisor/log/', Supervisor.as_view(context='log-all-device')),
    path('supervisor/log/<str:deviceID/', Supervisor.as_view(context='log-device')),
     
]