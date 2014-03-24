@rem Copyright (C) 2013 Petar Pofuk. All rights reserved.
@rem Use of this source code is governed by a BSD-style license that can be
@rem found in the LICENSE file.

call vsvars32.bat
cd ninja
@python "%~dp0bootstrap.py" %*
pause
