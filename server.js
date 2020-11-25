const MiServer = require("mimi-server");
const express = require("express");
const path = require("path");

const { app, server } = new MiServer({
	port: 8080,
	static: path.join(__dirname, "public")
});

const struct = require("python-struct");
const MagVar = require("magvar");

const message = new Buffer.from("RPOS\0");

// Routing
app.use("/js/jquery.slim.min.js", express.static(path.join(__dirname, "node_modules/jquery/dist/jquery.slim.min.js")));

const WebSocket = require("ws");
const wss = new WebSocket.Server({
	clientTracking: true,
	maxPayload: 1300,
	server
});

wss.on("connection", ws => {

	ws.on("error", error => {
		console.error("[ERROR] " + error);
	});
});

wss.on("error", error => {
	console.error("[ERROR] " + error);
});

const client = require("dgram").createSocket("udp4", data => {
	const [dat_lon, dat_lat, dat_ele, y_agl_mtr, veh_the_loc, veh_psi_loc, veh_phi_loc, vx_wrl, vy_wrl, vz_wrl, Prad, Qrad, Rrad] = struct.unpack("<dddffffffffff", data.slice(5));
	const MagneticVariation = MagVar.get(dat_lat, dat_lon);
	wss.clients.forEach(client => {
		if (client.readyState === WebSocket.OPEN) client.send(JSON.stringify({
			speed: Math.hypot(vx_wrl, vz_wrl),
			roll: veh_phi_loc,
			pitch: veh_the_loc,
			z: dat_ele,
			yaw: Rrad,
			heading: veh_psi_loc - MagneticVariation,
			vspeed: vy_wrl
		}));
	});
});

client.send(message, 0, message.length, 49000, "127.0.0.1", () => {});
