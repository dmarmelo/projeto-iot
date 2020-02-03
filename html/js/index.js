'use strict';

let config;

function loadConfig() {
    $.ajax({
        type: "GET",
        url: "/config",
        contentType: "text/plain; charset=utf-8",
        dataType: "json",
        success: function (response) {
            config = response;
            fillConfig();
        },
        error: function () {
            alert("Configuration load failed.")
        },
        timeout: 2000
    });
}

function fillConfig() {
    if (!config) return;
    $('#node-id').val(config.nodeId);
    $('#building').val(config.building);
    $('#room').val(config.room);

    $('#wifi-ssid').val(config.wifiSSID);
    $('#wifi-password').val(config.wifiPassword);
   
    $('#mqtt-ip').val(config.mqttIp);
    $('#mqtt-port').val(config.mqttPort);
    $('#mqtt-username').val(config.mqttUsername);
    $('#mqtt-password').val(config.mqttPassword);
    $('#mqtt-topic').val(config.mqttTopic);
}

function storeConfig() {
    $.ajax({
        type: "POST",
        url: "/save-config",
        dataType: "json",
        contentType: "application/json",
        data: JSON.stringify(config),
        success: function (response) {
            config = response;
            fillConfig();
            alert("Configuration Stored")
        },
        error: function () {
            alert("Unable to save current configuration, please try again.")
        },
        timeout: 2000
    });
}

function saveConfig() {
    config.nodeId = $('#node-id').val().trim();
    config.building = $('#building').val().trim();
    config.room = $('#room').val().trim();

    config.wifiSSID = $('#wifi-ssid').val().trim();
    config.wifiPassword = $('#wifi-password').val().trim();
   
    config.mqttIp = $('#mqtt-ip').val().trim();
    config.mqttPort = $('#mqtt-port').val().trim();
    config.mqttUsername = $('#mqtt-username').val().trim();
    config.mqttPassword = $('#mqtt-password').val().trim();
    config.mqttTopic = $('#mqtt-topic').val().trim();

    storeConfig();
}

$(document).ready(function () {
    loadConfig();
});
