//
// Copyright 2019 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// EGLRecordableTest.cpp:
//   Tests of EGL_ANDROID_recordable extension

#include <gtest/gtest.h>

#include "test_utils/ANGLETest.h"
#include "test_utils/angle_test_configs.h"
#include "util/EGLWindow.h"

using namespace angle;

namespace angle
{
class EGLRecordableTest : public ANGLETest
{
  protected:
    EGLRecordableTest() {}
};

// Test that the extension is always available (it is implemented in ANGLE's frontend).
TEST_P(EGLRecordableTest, ExtensionAlwaysAvailable)
{
    EGLDisplay display = getEGLWindow()->getDisplay();
    ASSERT_TRUE(ANGLETest::eglDisplayExtensionEnabled(display, "EGL_ANDROID_recordable"));
}

// Check that the default message filters and callbacks are correct
TEST_P(EGLRecordableTest, CheckAllContexts)
{
    EGLDisplay display = getEGLWindow()->getDisplay();
    ANGLE_SKIP_TEST_IF(!ANGLETest::eglDisplayExtensionEnabled(display, "EGL_ANDROID_recordable"));

    int nConfigs = 0;
    ASSERT_EGL_TRUE(eglGetConfigs(display, nullptr, 0, &nConfigs));
    ASSERT_NE(nConfigs, 0);

    int nReturnedConfigs = 0;
    std::vector<EGLConfig> configs(nConfigs);
    ASSERT_EGL_TRUE(eglGetConfigs(display, configs.data(), nConfigs, &nReturnedConfigs));
    ASSERT_EQ(nConfigs, nReturnedConfigs);

    for (EGLConfig config : configs)
    {
        EGLint isRecordable;
        eglGetConfigAttrib(display, config, EGL_RECORDABLE_ANDROID, &isRecordable);
        ASSERT_EGL_FALSE(isRecordable);
    }

    const EGLint configAttributes[] = {
        EGL_RED_SIZE,     EGL_DONT_CARE,  EGL_GREEN_SIZE,         EGL_DONT_CARE,  EGL_BLUE_SIZE,
        EGL_DONT_CARE,    EGL_ALPHA_SIZE, EGL_DONT_CARE,          EGL_DEPTH_SIZE, EGL_DONT_CARE,
        EGL_STENCIL_SIZE, EGL_DONT_CARE,  EGL_RECORDABLE_ANDROID, EGL_FALSE,      EGL_NONE};
    EGLint configCount;
    ASSERT_EGL_TRUE(
        eglChooseConfig(display, configAttributes, configs.data(), configs.size(), &configCount));
    ASSERT_EGL_SUCCESS();
}

}  // namespace angle

ANGLE_INSTANTIATE_TEST(EGLRecordableTest, ES2_D3D11(), ES2_OPENGL(), ES2_VULKAN());
