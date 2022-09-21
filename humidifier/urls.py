
from django.contrib import admin
from django.urls import path, include
from django.conf import settings
from django.conf.urls.static import static

urlpatterns = [
    path('supervisor/', admin.site.urls),
    path('', include('apps.urls'))
] + static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)
