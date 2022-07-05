

const coap = require('coap')
// const target = "coap://localhost:5683"
const target = "coap://coap.me:5683/test"
const req = coap.request(target)

// target.pipe(process.stdout)

req.on('response', (res) => {
    // res.pipe(process.stdout)
    console.log(res.payload.toString())
    // console.log(res.payload)
})

req.end()

