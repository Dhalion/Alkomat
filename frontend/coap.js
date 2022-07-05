
const coap = require('coap')
const target = "coap://coap.me:5683/test"


async function refresh() {
    const req = coap.request(target)

    req.on('response', (res) => {
        // res.pipe(process.stdout)
        console.log(res.payload.toString());
        showResponse(res.payload.toString());
        // console.log(res.payload)
    })
    req.end()
}

function showResponse(res) {
    document.getElementById('response').innerText = res;
}