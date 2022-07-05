const base_url = 'http://alkomat/'
// url = "https://gorest.co.in/public/v2/todos"

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

function displayStatus(status) {
    document.getElementById("status").innerText = status;
}

function displayWeight(weight) {
    document.getElementById("weight").innerText = weight + "g";
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


setInterval(getStatus, 1000);
setInterval(getWeight, 200);