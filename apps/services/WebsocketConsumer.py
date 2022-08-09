import json, random
from channels.generic.websocket import AsyncWebsocketConsumer, WebsocketConsumer, AsyncJsonWebsocketConsumer
from asgiref.sync import sync_to_async
from channels.consumer import AsyncConsumer
from asgiref.sync import async_to_sync


class PrivateWebsocket(AsyncWebsocketConsumer):

    deviceID = ''
    async def connect(self):

        self.deviceID = self.scope['url_route']['kwargs']['deviceID']
        await self.channel_layer.group_add(
            f'device_{self.deviceID}',
            self.channel_name
        )
        await self.accept()


    async def disconnect(self, close_code):
        # Leave room group
        await self.channel_layer.group_discard(
            f'device_{self.deviceID}',
            self.channel_name
        )

    async def device_incoming_post(self, event):
        data = event['data']
        # Send message to WebSocket
        await self.send(
            text_data=json.dumps({
            'data': json.loads(data)
        }))

