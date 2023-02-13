var openModalBtn = document.getElementById("add-device-item");
var modal = document.getElementById("main-modal");
var inputDeviceID = document.getElementById("id_deviceID");
var submitModalBtn = document.getElementById("modal-btn-submit");
var badgeInfo = document.getElementById("badge-info");
var badgeInfoText = document.getElementById("badge-info -text");
var tmpResp = ''
var csrfToken = document.getElementsByName('csrfmiddlewaretoken').value; 
function preLoaderPage(){
    inputDeviceID.value = '';
}

function reload(){
    window.location.href = '/monitoring/';  
}

function disableModal(){
    submitModalBtn.value = 'Loading..';
    submitModalBtn.disabled = true;
    submitModalBtn.style.backgroundColor = 'gray';
}

function enableModal(){
    submitModalBtn.value = 'Submit';
    submitModalBtn.disabled = false;
    submitModalBtn.style.backgroundColor = 'rgb(17, 163, 156)';
    inputDeviceID.value = '';
}

// Fungsi untuk register device baru
function register(deviceID) {
    alert(csrfToken);
    $.ajax({
        type: "POST",
        url: "/service/linking/",
        headers: {
            "X-CSRFToken": csrfToken,
            "Content-Type": "application/json",
        },
        data: {
            "deviceID": deviceID,
        },
        success: function (data) {
            tmpResp = (JSON.parse(data));
            

            switch (tmpResp['flag']){
                case 'danger':
                    // badgeInfo.style.backgroundColor = 'rgb(179, 52, 6)';
                    alert('[ERROR] : ' + tmpResp['message']);
                    break;
                case 'warning':
                    // badgeInfo.style.backgroundColor = 'rgb(170, 152, 14)';
                    alert('[WARNING] : ' + tmpResp['message']);
                    break;
                case 'info':
                    // badgeInfo.style.backgroundColor = 'rgb(17, 163, 156)';
                    alert(tmpResp['message'] + ' Mohon Tunggu....');
                    window.setTimeout(reload, 2500);
                    break;
            }

            // modal.style.display = 'none';
            // badgeInfoText.textContent = tmpResp['message'];
            // badgeInfo.style.display = 'initial';
            
            
        },
        failure: function (data) {
            // badgeInfo.style.backgroundColor = 'red';
            // badgeInfoText.textContent = 'Terdapat kesalahan jaringan ...';
            // badgeInfo.style.display = 'initial';
        },
        
    });
    enableModal();
}

submitModalBtn.onclick = function (){
    disableModal();
    register(inputDeviceID.value);
}

openModalBtn.onclick = function () {
    modal.style.display = "block";
}

window.onclick = function (event) {
    if (event.target == modal) {
        modal.style.display = "none";
    }
}

// Call init function
preLoaderPage();