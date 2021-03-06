/*
 * Copyright (c) 2005, 2010, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

/*
 * A DLL which is loaded by Java applications and used to map
 * between Java applications and native Win32 window handles.
 */

#include "com_sun_java_accessibility_AccessBridge.h" // programatically generated by JNI

#include <windows.h>
#include <stdio.h>

#include <jawt.h>
#include <win32/jawt_md.h>

// ---------------------------------------------------------------------------

extern "C" {
        /**
         * DllMain - where Windows executables will load/unload us
         *
         */
        BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved) {

                return TRUE;
        }


        /*
         * Map a HWND to a Java component
         *
         * Class:     com_sun_java_accessibility_AccessBridge
         * Method:    jawtGetComponentFromNativeWindowHandle
         * Signature: (I)Ljava/awt/Component;
         */
        JNIEXPORT jobject JNICALL
        Java_com_sun_java_accessibility_AccessBridge_jawtGetComponentFromNativeWindowHandle
                (JNIEnv *env, jobject callingObj, jint windowHandle) {

        JAWT awt;
        jboolean result;
            jobject component = (jobject)0;

        // Get the AWT
        awt.version = JAWT_VERSION_1_4;
        result = JAWT_GetAWT(env, &awt);
        if (result == JNI_FALSE) {
                        return (jobject)0;
                }

        // Get the component
        return awt.GetComponent(env, (void *)windowHandle);
        }


        /*
         * Map a Java component to a HWND
         *
         * Class:     com_sun_java_accessibility_AccessBridge
         * Method:    jawtGetNativeWindowHandleFromComponent
         * Signature: (Ljava/awt/Component;)I
         */
        JNIEXPORT jint JNICALL
        Java_com_sun_java_accessibility_AccessBridge_jawtGetNativeWindowHandleFromComponent
                (JNIEnv *env, jobject callingObj, jobject component)
        {

        JAWT awt;
        JAWT_DrawingSurface* ds;
        JAWT_DrawingSurfaceInfo* dsi;
        JAWT_Win32DrawingSurfaceInfo* dsi_win;
        jboolean result;
        // jint lock;
            jint windowHandle = -1;

        // Get the AWT
        awt.version = JAWT_VERSION_1_4;
        result = JAWT_GetAWT(env, &awt);
        if (result == JNI_FALSE) {
                        return -1;
                }

        // Get the drawing surface
        ds = awt.GetDrawingSurface(env, component);
        if (ds == NULL) {
                        return -1;
                }

                /*
                 * Should not be necessary.
                 *
        // Lock the drawing surface
        lock = ds->Lock(ds);
        if ((lock & JAWT_LOCK_ERROR) != 0) {
                        return -1;
                }
                 */

        // Get the drawing surface info
        dsi = ds->GetDrawingSurfaceInfo(ds);

        // Get the platform-specific drawing info
        dsi_win = (JAWT_Win32DrawingSurfaceInfo *)dsi->platformInfo;

                // Get the window handle
                windowHandle = (jint)dsi_win->hwnd;

        // Free the drawing surface info
        ds->FreeDrawingSurfaceInfo(dsi);

                /*
        // Unlock the drawing surface
        ds->Unlock(ds);
                */

        // Free the drawing surface
        awt.FreeDrawingSurface(ds);

                return windowHandle;
        }
}
