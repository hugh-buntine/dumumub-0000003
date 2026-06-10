#pragma once
#include <PluginProcessor.h>

[[maybe_unused]] static void runWithinPluginEditor (const std::function<void (DUMUMUB003AudioProcessor& plugin)>& testCode)
{
    DUMUMUB003AudioProcessor plugin;
    const auto editor = plugin.createEditorIfNeeded();

    testCode (plugin);

    plugin.editorBeingDeleted (editor);
    delete editor;
}
