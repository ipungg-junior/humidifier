from django.urls import path
from . import WebsocketConsumer

webSocket_urlpattern = [
    path('streaming/<int:deviceID>/', WebsocketConsumer.PrivateWebsocket.as_asgi())
]