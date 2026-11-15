// Visual server globals interface for the engine. This file implements the
// VisualServerGlobals class, which provides the API for managing global
// resources and references to various visual server components such as the
// canvas, viewport, and scene.
#include "visual_server_globals.h"

RasterizerStorage *VisualServerGlobals::storage = NULL;
RasterizerCanvas *VisualServerGlobals::canvas_render = NULL;
RasterizerScene *VisualServerGlobals::scene_render = NULL;
Rasterizer *VisualServerGlobals::rasterizer = NULL;

VisualServerCanvas *VisualServerGlobals::canvas = NULL;
VisualServerViewport *VisualServerGlobals::viewport = NULL;
VisualServerScene *VisualServerGlobals::scene = NULL;
