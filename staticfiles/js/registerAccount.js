var inEmail = document.getElementById('email-input');
var inPass = document.getElementById('password-input');
var inKonfirPass = document.getElementById('konfirmasi-password-input');
var btnRegister = document.getElementById('btn-register');
var csrf_token = document.getElementsByName('csrfmiddlewaretoken')[0].value;


btnRegister.addEventListener('click', function(){
    var emailVal = inEmail.value;
    var passVal = inPass.value;
    var konfirPassVal = inKonfirPass.value;
    if (passVal == konfirPassVal){
        if(passVal.length < 8){
            alert("Buat password minimal 8 karakter.");
        }else{
            
            var res = fetch("/account/register/", {
                method: "POST",
                headers: {
                    'Content-Type': 'application/json',
                    'X-CSRFToken': csrf_token,
                }, 
                body: JSON.stringify({
                    "email": emailVal,
                    "password": passVal,
                })
              }).then(res => {
                console.log(res.status);
                return res;
            })
            
        }
    }else{
        alert("Password tidak sama.");
    }
})
