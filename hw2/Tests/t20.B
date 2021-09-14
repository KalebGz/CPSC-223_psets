ff vs ffd [ff = 2, ffd = 3, bf = bfd = opt = 2]
-ff  2
-ffd 3
-opt 2
---
bf < ff [ff = ffd = 3, bf = bfd = opt = 2]
-bf  2
-ff  3
-opt 2
---
bfd < bf [ff = ffd = bf = 3, bfd = opt = 2]
-bfd 2
-bf  3
-opt 2
---
ffd < bfd [ff = ffd = 2, bf = bfd = 3, opt = 2]
-ffd 2
-bfd 3
-opt 2
---
ffd < bfd [ff = 3, ffd = 2, bf = bfd = 3, opt = 2]
-ffd 2
-bfd 3
-opt 2
