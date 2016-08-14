if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
imap <silent> <F8> :TagbarToggle
imap <F3> a
imap <F2> 
imap <F1> 
map  :set spell spelllang=es
map  :set nospell
map  :tab split:exec("tag ".expand("<cword>"))
nmap \ihn :IHN
nmap \ih :IHS
noremap \a :A 
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
nnoremap <silent> <F8> :TagbarToggle
noremap <F9> :call HexMe()
map <F4> :emenu 
map <F3> a
map <F2> i
map <F1> 
imap \ihn :IHN
imap \ih :IHS
inoremap \a :A 
let &cpo=s:cpo_save
unlet s:cpo_save
set background=dark
set backspace=indent,eol,start
set fileencodings=ucs-bom,utf-8,default,latin1
set grepprg=grep\ -nH\ $*
set helplang=en
set history=50
set hlsearch
set ignorecase
set nomodeline
set printoptions=paper:a4
set ruler
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim73,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set scrolloff=5
set showfulltag
set sidescroll=1
set sidescrolloff=10
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set noswapfile
set wildcharm=26
set wildmenu
" vim: set ft=vim :
