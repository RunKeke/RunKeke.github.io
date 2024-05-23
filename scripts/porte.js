const mqttUser2 = "porte";
const mqttPassword2 = "Porte123";

const client2 = new Paho.MQTT.Client(mqttServer, mqttPort, "clientId-" + parseInt(Math.random() * 100, 10));

function onMessageArrived(message) {
    console.log("Message reçu: " + message.payloadString);
    document.getElementById("data2").innerText = "La porte de la maison est " + message.payloadString;
}

function onConnect() {
    console.log("Connecté au serveur MQTT");
    client2.subscribe("home/porte");
}

function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("Connexion perdue: " + responseObject.errorMessage);
    }
}

client2.onMessageArrived = onMessageArrived;
client2.onConnectionLost = onConnectionLost;

client2.connect({
    onSuccess: onConnect,
    onFailure: function(e) { console.log("Échec de la connexion: " + e.errorMessage); },
    userName: mqttUser2,
    password: mqttPassword2,
    useSSL: true
});