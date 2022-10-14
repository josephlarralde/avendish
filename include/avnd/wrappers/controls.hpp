#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <avnd/common/for_nth.hpp>
#include <avnd/wrappers/avnd.hpp>
#include <avnd/wrappers/ranges.hpp>
#include <avnd/wrappers/widgets.hpp>
#include <cmath>

#include <string>

namespace avnd
{
/**
 * @brief Used to set the initial value for controls when
 * the plug-in is first loaded.
 */
template <typename F>
static constexpr void init_controls(avnd::effect_container<F>& effect)
{
  for(auto& state : effect.full_state())
  {
    avnd::for_each_field_ref(state.inputs, [&]<typename T>(T& ctl) {
      if constexpr(avnd::has_range<T>)
      {
        constexpr auto c = avnd::get_range<T>();
        if_possible(ctl.value = c.init)                    // Default case
            else if_possible(ctl.value = c.values[c.init]) // For string enums

            if_possible(ctl.update(state.effect));
      }
    });
  }
}

/**
 * @brief Used for the case where the "host" can handle any
 * range of value: the plug-in will report the range,
 * and then we just have to clip to be safe
 */
template <typename T>
static constexpr void apply_control(T& ctl, std::floating_point auto v)
{
  // Apply the value
  ctl.value = v;

  // Clamp
  if constexpr(avnd::parameter_with_minmax_range<T>)
  {
    constexpr auto c = avnd::get_range<T>();
    if(ctl.value < c.min)
      ctl.value = c.min;
    else if(ctl.value > c.max)
      ctl.value = c.max;
  }
  else if constexpr(avnd::parameter_with_values_range<T>)
  {
    ctl.value = range_value<T>(closest_value_index<T>(ctl.value));
  }
}

template <typename T>
static constexpr void apply_control(T& ctl, avnd::string_ish auto v)
{
  // Apply the value
  if constexpr(avnd::parameter_with_values_range<T>)
  {
    ctl.value = range_value<T>(closest_value_index<T>(ctl.value));
  }
  else
  {
    ctl.value = v;
  }
}
/*
static void apply_control(auto& ctl, std::string&& v)
{
  // Apply the value
  ctl.value = std::move(v);

  // Clamp in range
  TODO;
}
*/
/**
 * @brief Used for the case where the "host" works in a fixed [0. ; 1.] range
 */
template <avnd::float_parameter T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  if constexpr(avnd::parameter_with_minmax_range<T>)
  {
    constexpr auto c = avnd::get_range<T>();
    return c.min + v * (c.max - c.min);
  }
  else if constexpr(avnd::parameter_with_values_range<T>)
  {
    // Here we just map from / to the index of the value

    // [0;1] -> [0;N[
    const auto mapped = map_index_from_01<avnd::get_enum_choices_count<T>()>(v);

    // [0;N[ -> actual value
    return range_value<T>(mapped);
  }
  else
  {
    return v;
  }
}

template <avnd::int_parameter T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  if constexpr(avnd::parameter_with_minmax_range<T>)
  {
    constexpr auto c = avnd::get_range<T>();
    return c.min + v * (c.max - c.min);
  }
  else if constexpr(avnd::parameter_with_values_range<T>)
  {
    // Here we just map from / to the index of the value

    // [0;1] -> [0;N[
    const auto mapped = map_index_from_01<avnd::get_enum_choices_count<T>()>(v);

    // [0;N[ -> actual value
    return range_value<T>(mapped);
  }
  else
  {
    return v;
  }
}

template <avnd::string_parameter T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  return decltype(T::value){};
}

template <avnd::bool_parameter T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  return v < 0.5 ? false : true;
}

template <avnd::enum_parameter T>
static constexpr auto map_control_from_01(std::floating_point auto v)
{
  return static_cast<decltype(T::value)>(
      map_index_from_01<avnd::get_enum_choices_count<T>()>(v));
}

template <typename T>
static constexpr auto map_control_from_01(std::floating_point auto v) = delete;
// {
//   static_assert(std::is_void_v<T>, "Error: unhandled control type");
// }

template <avnd::float_parameter T>
static constexpr auto map_control_to_01(const auto& value)
{
  // Apply the value
  double v{};
  if constexpr(avnd::parameter_with_minmax_range<T>)
  {
    constexpr auto c = avnd::get_range<T>();

    v = (value - c.min) / double(c.max - c.min);
  }
  else if constexpr(avnd::parameter_with_values_range<T>)
  {
    const int idx = closest_value_index<T>(value);
    v = map_index_to_01<avnd::get_enum_choices_count<T>()>(idx);
  }
  else
  {
    v = value;
  }
  return v;
}

template <avnd::int_parameter T>
static constexpr auto map_control_to_01(const auto& value)
{
  // Apply the value
  double v{};
  if constexpr(avnd::parameter_with_minmax_range<T>)
  {
    // TODO generalize
    static_assert(avnd::get_range<T>().max != avnd::get_range<T>().min);
    constexpr auto c = avnd::get_range<T>();

    v = (value - c.min) / double(c.max - c.min);
  }
  else if constexpr(avnd::parameter_with_values_range<T>)
  {
    const int idx = closest_value_index<T>(value);
    v = map_index_to_01<avnd::get_enum_choices_count<T>()>(idx);
  }
  else
  {
    v = value;
  }
  return v;
}

template <avnd::string_parameter T>
static constexpr auto map_control_to_01(const auto& value)
{
  double v{};
  // TODO if we have a range, use it. Otherwise.. ?
  return v;
}

template <avnd::bool_parameter T>
static constexpr auto map_control_to_01(const auto& value)
{
  return value ? 1. : 0.;
}

template <avnd::enum_parameter T>
static constexpr auto map_control_to_01(const auto& value)
{
  return map_index_to_01<avnd::get_enum_choices_count<T>()>(static_cast<int>(value));
}

template <typename T>
static constexpr auto map_control_to_01(const auto& value) = delete;
// {
//   static_assert(std::is_void_v<T>, "Error: unhandled control type");
// }

template <avnd::parameter T>
requires requires(T t)
{
  map_control_to_01(t.value);
}
static constexpr auto map_control_to_01(const T& ctl)
{
  return map_control_to_01<T>(ctl.value);
}
}
