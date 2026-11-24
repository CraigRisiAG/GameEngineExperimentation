# This file provides configuration for the GDNative module, including build
# settings and documentation paths. 
def can_build(env, platform):
    return True

def configure(env):
    env.use_ptrcall = True

def get_doc_classes():
    return [
        "@NativeScript",
        "ARVRInterfaceGDNative",
        "GDNative",
        "GDNativeLibrary",
        "MultiplayerPeerGDNative",
        "NativeScript",
        "PacketPeerGDNative",
        "PluginScript",
        "StreamPeerGDNative",
        "VideoStreamGDNative",
        "WebRTCPeerConnectionGDNative",
        "WebRTCDataChannelGDNative",
    ]

def get_doc_path():
    return "doc_classes"
