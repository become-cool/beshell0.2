const net = require('net');
const client = net.createConnection({ host:'192.168.0.112', port: 8022 }, () => {
    console.log('已连接到服务器');
    client.write(new Uint8Array([5, 18, 0, 1, 20, 99, 111, 110, 115, 111, 108, 101, 46, 108, 111, 103, 40, 39, 104, 101, 108, 108, 111, 39, 41, 92]));
});
client.on('data', (data) => {
    console.log(data.toString());
});
client.on('end', () => {
    console.log('已从服务器断开');
});