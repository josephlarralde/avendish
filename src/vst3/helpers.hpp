#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <vst3/metadata.hpp>

#include <common/widechar.hpp>

#include <base/source/fstreamer.h>
#include <pluginterfaces/vst/ivstattributes.h>
#include <pluginterfaces/vst/vstpresetkeys.h>
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/base/ustring.h>

#include <algorithm>
#include <string_view>

namespace stv3
{
template <std::size_t M>
auto setStr(TChar (&field)[M], std::u16string_view text)
{
  std::copy_n(text.data(), text.size(), field);
}

template <std::size_t M>
auto setStr(TChar (&field)[M], std::string_view text)
{
  avnd::utf8_to_utf16(text.begin(), text.end(), field);
}

inline
Steinberg::tresult isProjectState (Steinberg::IBStream* state)
{
  using namespace Steinberg;
  using namespace Steinberg::Vst;
  if (!state)
    return kInvalidArgument;

  FUnknownPtr<IStreamAttributes> stream (state);
  if (!stream)
    return kNotImplemented;

  if (IAttributeList* list = stream->getAttributes ())
  {
    // get the current type (project/Default..) of this state
    String128 string = {0};
    if (list->getString (PresetAttributes::kStateType, string, 128 * sizeof (TChar)) ==
        kResultTrue)
    {
      UString128 tmp (string);
      char ascii[128];
      tmp.toAscii (ascii, 128);
      if (!strncmp (ascii, StateType::kProject, strlen (StateType::kProject)))
      {
        return kResultTrue;
      }
      return kResultFalse;
    }
  }
  return kNotImplemented;
}

}