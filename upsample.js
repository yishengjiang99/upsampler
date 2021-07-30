import { wasmbin } from "./wasbin.js";
const module = new WebAssembly.Module(wasmbin);

export class Upsampler {
  constructor(ctx, memory) {
    this.instance = new WebAssembly.Instance(module, {
      env: {
        powf: Math.pow,
        memoryBase: 0,
        __memory_base: 0,
        memory: memory,
        onval: (frac, pos) => {},
      },
    });
    this.heap = memory;
  }
  upsample(input, ratio) {
    const sampleRef = 0;
    this.ref = this.instance.exports.init(sampleRef, input.length);
    new Float32Array(this.heap.buffer, sampleRef, input.length).set(input);
    this.instance.exports.setRatio(this.ref, ratio);
    return [
      new Uint32Array(this.heap.buffer, this.ref, 4),
      new Float32Array(this.heap.buffer, this.ref + 20, 1),
    ];
  }
  process(outputRef) {
    this.instance.exports.upsample_wave_table(this.ref, outputRef);
    return [
      new Uint32Array(this.heap.buffer, this.ref, 4),
      new Float32Array(this.heap.buffer, this.ref + 20, 1),
    ];
  }
}
