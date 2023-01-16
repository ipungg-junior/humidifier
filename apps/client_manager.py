from django.contrib.auth.base_user import BaseUserManager
from django.utils.translation import ugettext_lazy as _
from django.contrib.auth import get_user_model

class ClientManager(BaseUserManager):
    
    # Create client account 
    def create_user(self, email, password, **extra_fields):
        '''
            Fungsi untuk membuat user baru dengan param (email, password), jika berhasil return objek user,
            jika gagal maka return None
        '''
        try:
            if not email:
                raise ValueError(_('The Email must be set'))
            email = self.normalize_email(email)
            user = get_user_model().objects.create(email=email, **extra_fields)
            user.set_password(password)
            user.save()
            return user
        except:
            return None

    # Superuser Account
    def create_superuser(self, email, password, **extra_fields):
        extra_fields.setdefault('is_staff', True)
        extra_fields.setdefault('is_superuser', True)
        extra_fields.setdefault('is_active', True)

        if extra_fields.get('is_staff') is not True:
            raise ValueError(_('Superuser must have is_staff=True.'))
        if extra_fields.get('is_superuser') is not True:
            raise ValueError(_('Superuser must have is_superuser=True.'))
        return self.create_user(email, password, **extra_fields)
