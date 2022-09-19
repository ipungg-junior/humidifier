
from django.contrib import admin
from django.urls import path, include
from django.conf import settings
from django.conf.urls.static import static
from .views import Log

urlpatterns = [
    path('supervisor/', admin.site.urls),
    path('supervisor/log/', Log.as_view(context='log-all-device')),
    path('supervisor/log/<str:deviceID/', Log.as_view(context='log-device')),
    path('', include('apps.urls'))
] + static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)
