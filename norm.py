#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

dir_frame = './frames'
frame_fns = os.listdir(dir_frame)
for fn in frame_fns:
    chunks = fn.split('.')
    frame_cnt = int(chunks[0])
    ext = chunks[1]
    os.rename(dir_frame + '/' + fn,
              dir_frame + '/' + str(frame_cnt) + '.' + ext)
