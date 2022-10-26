### Tests

The DU unit tests suite is available in the following git tree. 

```bash
git://git.ideasonboard.com/renesas/kms-tests.git
```

Additionally, instructions are available to perform manual tests using modetest, [kmsxx](https://elinux.org/R-Car/Tests:KMS) or [DMA buffer sharing](https://elinux.org/R-Car/Tests:KMS-dmabuf). 

### Testing Interlaced Pipelines

To configure kmstest to use an interlaced pipeline, you must specify the connector, and the resolution you would like to test. For example, to test a 1080i resolution on connector 1:

```bash
kmstest -c 1 -r 1920x1080i
```

or to validate a moving image:

```bash
kmstest -c 1 -r 1920x1080i --flip
```

### References

[R-Car/Tests:KMS-dmabuf](https://elinux.org/R-Car/Tests:KMS-dmabuf)
