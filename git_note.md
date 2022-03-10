https://www.bilibili.com/video/BV1db4y1d79C

克隆仓库 `git clone <git地址>`
		初始化仓库：`git init` 

添加文件到暂存区：`git add -A`
		把暂存区的文件提交到仓库：`git commit -m "提交信息"`
		查看提交的历史记录：`git log --stat`

工作区回滚：`git checkout <filename>`
		撤销最后一次提交：`git reset HEAD^1`

以当前分支为基础新建分支：`git checkout -b <branchname>`
		列举所有的分支：`git branch`
		单纯地切换到某个分支：`git checkout <branchname>`
		删掉特定的分支：`git branch -D <branchname>`
		合并分支：`git merge <branchname>`



本地同步远程仓库 `git remote add origin <https://.....git>`

重命名 `git branch -M main`

拉取远程 `		git pull --rebase origin main`  （如果本地还没commit可以先 `git pull`）

再push `git push -u origin main`



## Q1:

一开始git push的时候报错 error: failed to push some refs to 'https://github.com/...

原因是远程库与本地库不一致造成的

需要先`git pull --rebase origin main`` 将刚刚commit的内容保存为补丁

再``git push -u origin main``



## Q2:

pull或push的时候报错 [OpenSSL](https://so.csdn.net/so/search?q=OpenSSL&spm=1001.2101.3001.7020) SSL_read: Connection was reset, errno 10054

方法：

更改网络认证设置 `git config --global http.sslVerify "false"`

增加缓冲 `git config http.postBuffer 524288000`



