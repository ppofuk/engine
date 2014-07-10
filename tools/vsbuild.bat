@rem Copyright (C) 2013 Petar Pofuk. All rights reserved.
@rem Use of this source code is governed by a BSD-style license that can be
@rem found in the LICENSE file.

call enviroment.bat
@python "%~dp0vsbuild.py" %*
pause
