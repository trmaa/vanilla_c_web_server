// ---------- Global Variables ----------
let audio_ctx = new (window.AudioContext || window.webkitAudioContext)();
const active_oscillators = new Map();
let sustain = false;

// ---------- Audio Playback ----------
function ensure_audio_context() {
	if (!audio_ctx) {
		audio_ctx = new AudioContext();
	} else if (audio_ctx.state === "suspended") {
		audio_ctx.resume();
	}
}

function play_note_start(freq, key_id) {
	ensure_audio_context();
	if (active_oscillators.has(key_id)) {
		try {
			active_oscillators.get(key_id).stop();
		} catch (e) {}
		active_oscillators.delete(key_id);
	}

	const osc = audio_ctx.createOscillator();
	const gain = audio_ctx.createGain();

	osc.type = "square";
	osc.frequency.value = freq;
	gain.gain.setValueAtTime(0.2, audio_ctx.currentTime);

	osc.connect(gain).connect(audio_ctx.destination);
	osc.start();

	active_oscillators.set(key_id, osc);

	const key = document.getElementById(key_id);
	if (key) key.classList.add("active");
}

function play_note_stop(key_id) {
	if (sustain) return;

	const osc = active_oscillators.get(key_id);
	if (osc) {
		try {
			osc.stop();
		} catch (e) {}
		active_oscillators.delete(key_id);
	}

	const key = document.getElementById(key_id);
	if (key) key.classList.remove("active");
}

function stop_all_notes() {
	for (const [key_id, osc] of active_oscillators.entries()) {
		try {
			osc.stop();
		} catch (e) {}
		const key = document.getElementById(key_id);
		if (key) key.classList.remove("active");
	}
	active_oscillators.clear();
}

function toggle_sustain() {
	sustain = !sustain;
	const btns = document.getElementsByClassName("sustain_btn");
	for (let i = 0; i < btns.length; i++) {	
		btns[i].textContent = `Sustain: ${sustain ? "ON" : "OFF"}`;
	}

	if (!sustain) {
		stop_all_notes();
	}
}

function handle_touch_start(event, freq, key_id) {
	event.preventDefault();
	play_note_start(freq, key_id);
}
function handle_touch_end(event, key_id) {
	event.preventDefault();
	play_note_stop(key_id);
}
