import os
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'humidifier.settings')
from django.core.asgi import get_asgi_application
django_asgi_app = get_asgi_application()
import apps.services.WebsocketRoute as WebsocketRoute
## Channels conf
from channels.auth import AuthMiddlewareStack
from channels.routing import ProtocolTypeRouter, URLRouter

application = ProtocolTypeRouter(
    {
        "http"  : django_asgi_app,
        "websocket": AuthMiddlewareStack(URLRouter(WebsocketRoute.webSocket_urlpattern))
    }
)

