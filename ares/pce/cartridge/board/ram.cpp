struct RAM : Interface {
  using Interface::Interface;
  Memory::Readable<uint8> rom;
  Memory::Writable<uint8> ram;

  struct Debugger {
    maybe<RAM&> super;

    //debugger.cpp
    auto load(Node::Object) -> void;
    auto unload(Node::Object) -> void;

    struct Memory {
      Node::Memory ram;
    } memory;
  } debugger;

  auto load(Markup::Node document) -> void override {
    auto board = document["game/board"];
    Interface::load(rom, board["memory(type=ROM,content=Program)"]);
    Interface::load(ram, board["memory(type=RAM,content=Save)"]);

    debugger.super = *this;
    debugger.load(cartridge.node);
  }

  auto save(Markup::Node document) -> void override {
    auto board = document["game/board"];
    Interface::save(ram, board["memory(type=RAM,content=Save)"]);
  }

  auto unload() -> void override {
    debugger.unload(cartridge.node);
  }

  auto read(uint8 bank, uint13 address, uint8 data) -> uint8 override {
    if(bank >= 0x00 && bank <= 0x3f) {
      return rom.read(bank << 13 | address);
    }

    if(bank >= 0x40 && bank <= 0x43) {
      return ram.read(bank << 13 | address);
    }

    return data;
  }

  auto write(uint8 bank, uint13 address, uint8 data) -> void override {
    if(bank >= 0x40 && bank <= 0x43) {
      return ram.write(bank << 13 | address, data);
    }
  }

  auto power() -> void override {
  }

  auto serialize(serializer& s) -> void override {
    if(ram) ram.serialize(s);
  }
};
