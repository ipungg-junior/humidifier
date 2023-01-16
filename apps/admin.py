from django.contrib import admin
from .models import *
from django.contrib.auth.admin import UserAdmin


class CustomUserAdmin(UserAdmin):
    model = ClientAccount
    list_display = ('email', 'is_staff', 'is_active',)
    list_filter = ('email', 'is_staff', 'is_active',)
    fieldsets = (
        (None, {'fields': ('email', 'password',)}),
        ('Permissions', {'fields': ('is_staff', 'is_active')})
    )
    add_fieldsets = (
        (None, {
            'classes': ('wide',),
            'fields': ('email', 'password1', 'password2', 'is_staff', 'is_active')}
        ),
    )
    search_fields = ('email',)
    ordering = ('email',)




# Register your models here.
admin.site.register(StandaloneDeviceSession)
admin.site.register(DeviceSession)
admin.site.register(DeviceUsage)
admin.site.register(RegisterDeviceSegmentation)
admin.site.register(ClientDevice)
admin.site.register(ClientAccount, CustomUserAdmin)