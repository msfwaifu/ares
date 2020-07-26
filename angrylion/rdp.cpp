#include <n64/n64.hpp>

namespace ares::Nintendo64 {

RDP rdp;
#include "n64video.c"
#include "render.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto RDP::load(Node::Object parent) -> void {
  node = parent->append<Node::Component>("RDP");
}

auto RDP::unload() -> void {
  node = {};
}

auto RDP::main() -> void {
  step(93'750'000);
}

auto RDP::step(uint clocks) -> void {
  clock += clocks;
}

auto RDP::power() -> void {
  Thread::reset();
  command = {};
  set = {};
  io.bist = {};
  io.test = {};
  angrylion_rdp_init();
}

}
