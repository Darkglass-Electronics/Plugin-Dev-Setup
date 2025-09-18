// Example plugin for Anagram using rust
// Copyright (C) 2025 Filipe Coelho <falktx@darkglass.com>
// SPDX-License-Identifier: ISC

use lv2::prelude::*;

// --------------------------------------------------------------------------------------------------------------------
// definitions for this example plugin, matching the Darkglass Gain plugin

const MIN_GAIN_DB : f32 = -40.0;
const MAX_GAIN_DB : f32 = 20.0;

// --------------------------------------------------------------------------------------------------------------------
// dB to coefficient

fn db2coef(g: f32) -> f32
{
    if g > MIN_GAIN_DB {
        return 10_f32.powf(g * 0.05_f32)
    } else {
        return 0_f32
    };
}

// --------------------------------------------------------------------------------------------------------------------
// simple lowpass filtering for parameter smoothing

struct Filter {
    a0: f32,
    b1: f32,
    z1: f32,
    target: f32,
}

impl Filter {
    fn new(sample_rate: f64) -> Self {
        let frequency : f32 = 20.0 / sample_rate as f32;
        let b1 = (-2.0 * std::f32::consts::PI * frequency).exp();
        let a0 = 1.0 - b1;

        Self {
            a0: a0,
            b1: b1,
            z1: 0.0,
            target: db2coef(0_f32),
        }
    }

    fn next(&mut self) -> f32 {
        self.z1 = self.target * self.a0 + self.z1 * self.b1;
        return self.z1;
    }

    fn reset(&mut self) {
        self.z1 = 0.0;
    }
}

// --------------------------------------------------------------------------------------------------------------------
// Plugin implementation

#[derive(PortCollection)]
pub struct Ports {
    input: InputPort<Audio>,
    output: OutputPort<Audio>,
    enabled: InputPort<Control>,
    gain: InputPort<Control>,
    invert_polarity: InputPort<Control>,
}

#[uri("https://github.com/Darkglass-Electronics/Plugin-Dev-Setup#example-gain-rust")]
pub struct GainPlugin {
    smooth_gain: Filter,
}

impl Plugin for GainPlugin {
    type Ports = Ports;

    type InitFeatures = ();
    type AudioFeatures = ();

    fn new(plugin_info: &PluginInfo, _features: &mut ()) -> Option<Self> {
        Some(Self {
            smooth_gain: Filter::new(plugin_info.sample_rate()),
        })
    }

    fn activate(&mut self, _features: &mut ()) {
        self.smooth_gain.reset();
    }

    fn run(&mut self, ports: &mut Ports, _features: &mut (), _: u32) {
        // reuse bypass and gain parameter handling for this simple example
        // if enabled, set smooth gain as the coefficient from dB
        // if bypassed, set smooth gain to 0dB
        self.smooth_gain.target = if *ports.enabled > 0.5 {
            db2coef((*ports.gain).clamp(MIN_GAIN_DB, MAX_GAIN_DB))
        } else {
            db2coef(0_f32)
        };

        // polarity
        let polarity : f32 = if *ports.invert_polarity > 0.5 {
            -1.0
        } else {
            1.0
        };

        // apply gain and polarity
        for (in_frame, out_frame) in Iterator::zip(ports.input.iter(), ports.output.iter_mut()) {
            *out_frame = in_frame * self.smooth_gain.next() * polarity;
        }
    }
}

lv2_descriptors!(GainPlugin);
