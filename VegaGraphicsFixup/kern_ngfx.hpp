//
//  kern_ngfx.hpp
//  VegaGraphicsFixup based on NvidiaGraphicsFixup
//
//  Copyright © 2017 lvs1974. All rights reserved.
//  Copyright © 2018 JYA. All rights reserved.
//

#ifndef kern_ngfx_hpp
#define kern_ngfx_hpp

#include <Headers/kern_patcher.hpp>
#include <Library/LegacyIOService.h>

struct KextPatch {
  KernelPatcher::LookupPatch patch;
  uint32_t minKernel;
  uint32_t maxKernel;
};

class NGFX {
public:
  bool init();
  void deinit();

private:
  /**
   *  Patch kext if needed and prepare other patches
   *
   *  @param patcher KernelPatcher instance
   *  @param index   kinfo handle
   *  @param address kinfo load address
   *  @param size    kinfo memory size
   */
  void processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);

  /**
   *  SetAccelProperties callback type
   */
  using t_set_accel_properties = void (*) (IOService * that);


  /**
   *  Hooked methods / callbacks
   */
  static void nvAccelerator_SetAccelProperties(IOService* that);


  /**
   *  Trampolines for original method invocations
   */
  t_set_accel_properties      orgSetAccelProperties {nullptr};

  /**
   *  Apply kext patches for loaded kext index
   *
   *  @param patcher    KernelPatcher instance
   *  @param index      kinfo index
   *  @param patches    patch list
   *  @param patchesNum patch number
   */
  void applyPatches(KernelPatcher &patcher, size_t index, const KextPatch *patches, size_t patchesNum, const char *name);

  /**
   *  Current progress mask
   */
  struct ProcessingState {
    enum {
      NothingReady = 0,
      GraphicsDevicePolicyPatched = 1 << 0,
      GeForceRouted = 1 << 1,
      EverythingDone = GraphicsDevicePolicyPatched | GeForceRouted,
    };
  };
  int progressState {ProcessingState::NothingReady};
};

#endif /* kern_ngfx */

