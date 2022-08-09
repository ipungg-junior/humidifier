import os
import apps.services.WebsocketRoute as WebsocketRoute
from django.core.asgi import get_asgi_application

## Channels conf
from channels.auth import AuthMiddlewareStack
from channels.routing import ProtocolTypeRouter, URLRouter

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'humidifier.settings')
application = ProtocolTypeRouter(
    {
        "http"  : get_asgi_application(),
        "websocket": AuthMiddlewareStack(URLRouter(WebsocketRoute.webSocket_urlpattern))
    }
)