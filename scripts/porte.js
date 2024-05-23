const mqttServer = "264102e9ac51433ebbd21aaeb57014e9.s1.eu.hivemq.cloud";
const mqttPort = 8884;

const mqttUser = "porte";
const mqttPassword = "Porte123";

const client = new Paho.MQTT.Client(mqttServer, mqttPort, "clientId-" + parseInt(Math.random() * 100, 10));

function onMessageArrived(message) {
    console.log("Message reçu: " + message.payloadString);
    document.getElementById("data2").innerText = "La porte de la maison est " + message.payloadString;
}

function onConnect() {
    console.log("Connecté au serveur MQTT");
    client.subscribe("home/porte");
}

function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("Connexion perdue: " + responseObject.errorMessage);
    }
}

client.onMessageArrived = onMessageArrived;
client.onConnectionLost = onConnectionLost;

client.connect({
    onSuccess: onConnect,
    onFailure: function(e) { console.log("Échec de la connexion: " + e.errorMessage); },
    userName: mqttUser,
    password: mqttPassword,
    useSSL: true
});