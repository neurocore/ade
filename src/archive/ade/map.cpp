#include "map.h"

namespace ade {

// @formatter:off

Map::Memento Map::get_updates(const Map& old) const
{
  Memento memento;
  memento.name     = get_diffs(old.name,     name);
  memento.vertices = get_diffs(old.vertices, vertices);
  memento.lines    = get_diffs(old.lines,    lines);
  memento.sectors  = get_diffs(old.sectors,  sectors);
  memento.segments = get_diffs(old.segments, segments);
  memento.things   = get_diffs(old.things,   things);
  return memento;
}

void Map::undo_updates(const Memento& memento)
{
  name     = undo(name,     memento.name);
  vertices = undo(vertices, memento.vertices);
  lines    = undo(lines,    memento.lines);
  sectors  = undo(sectors,  memento.sectors);
  segments = undo(segments, memento.segments);
  things   = undo(things,   memento.things);
}

void Map::redo_updates(const Memento& memento)
{
  name     = redo(name,     memento.name);
  vertices = redo(vertices, memento.vertices);
  lines    = redo(lines,    memento.lines);
  sectors  = redo(sectors,  memento.sectors);
  segments = redo(segments, memento.segments);
  things   = redo(things,   memento.things);
}

// @formatter:on

}
