#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <avnd/concepts/processor.hpp>
#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>
#include <cmath>

namespace examples::helpers
{
struct PerSampleAsArgs
{
  avnd_meta(name, "Per-sample processing (args, helpers)")
  avnd_meta(c_name, "avnd_helpers_per_sample_as_args")
  avnd_meta(uuid, "92a1a42b-dc63-42f8-8d24-8cb9c046803a")

  float operator()(float input) { return std::tanh(input); }
};

struct PerSampleAsPorts
{
  avnd_meta(name, "Per-sample processing (ports, helpers)")
  avnd_meta(c_name, "avnd_helpers_per_sample_as_ports")
  avnd_meta(uuid, "31146933-c26b-4c21-a3f1-e991f70b520c")

  struct inputs
  {
    halp::audio_sample<"In", double> audio;
  };

  struct outputs
  {
    halp::audio_sample<"Out", double> audio;
  };

  void operator()(const inputs& ins, outputs& outs)
  {
    outs.audio = std::tanh(ins.audio);
  }
};

static_assert(avnd::monophonic_processor<double, PerSampleAsPorts>);
static_assert(avnd::mono_per_sample_port_processor<double, PerSampleAsPorts>);
static_assert(avnd::sample_port_processor<PerSampleAsPorts>);
static_assert(avnd::inputs_is_type<PerSampleAsPorts>);
static_assert(avnd::outputs_is_type<PerSampleAsPorts>);
}
