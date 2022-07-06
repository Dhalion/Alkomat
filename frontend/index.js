const base_url = 'http://alkomat/'
// url = "https://gorest.co.in/public/v2/todos"

let lastLogTimestamp;


async function getStatus() {
    const response = await fetch(base_url + "status");
    const data = await response.json();
    if (data.status.toLowerCase() === "healthy") {
        displayStatus(data.status)
    }
    else {displayStatus("Disconnected")}

}


async function getWeight() {
    const response = await fetch(base_url + "scale");
    const data = await response.json();
    displayWeight(data.weight);

}


async function getLogs() {
    const response = await fetch(base_url + "log");
    
    if (!response.ok) {
        const message = `An error has occured: ${response.status}`;
        throw new Error(message);
    }
    
    const data = await response.json();
    return data;
}




function updateLogs() {
    getLogs().then(data => {
        for (element in data) {
            for (field in element) {
                const log = data[field]["timestamp"] + ": " + data[field]["message"];
                addLog(log + "\n");
                
            }
        }
    })
}


function displayStatus(status) {
    document.getElementById("status").innerText = status;
}

function displayWeight(weight) {
    document.getElementById("weight").innerText = weight + "g";
}

function addLog(text) {
    const textarea = document.getElementById('logtext');
    textarea.value += text;
    textarea.scrollTop = textarea.scrollHeight;
}


async function fill() {
    const amount = document.getElementById("amount").value;
    const url = base_url + "pour?" + new URLSearchParams({ amount: amount, valve: 0 });
    console.log(url);
    const response = await fetch(url)


    console.log(response);
    alert("amount filled.");
}

async function valve(data){
    let url = base_url + "valve?" + new URLSearchParams({ "valve": data.id, action: "close" });
    if (data.action == 0) {
        console.log("Closing Valve", data.id);
        url = base_url + "valve?" + new URLSearchParams({ "valve": data.id, action: "close" });
        console.log(url);

    } else if (data.action == 1) {
        console.log("Opening Valve", data.id);
        url = base_url + "valve?" + new URLSearchParams({ "valve": data.id, action: "open" });
        console.log(url);
    }
    else {console.log("Error");}

    fetch(url);
}


function calibration_button() {
    console.log("Calibration");

    const mass = document.getElementById("mass_input").value;
    let request = base_url + "calibration?" +new URLSearchParams({"mass": mass});

    fetch(request);
}


function tare_button() {
    console.log("Tare");

    let request = base_url + "tare";

    fetch(request);
}


setInterval(getStatus, 1000);
setInterval(updateLogs, 300);
setInterval(getWeight, 200);