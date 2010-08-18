$Id: README.txt,v 1.2 2008/10/12 02:32:40 mikeyp Exp $

Description
===========
This profile is a dynamic multistep installer. It can install any number of additional packages which are kept in packgr/packages. The packages define a few simple hooks which return their info and tasks back to the installer. The installer then steps through the tasks and completes them as needed, or displays a form to the user based on the task type. Please see packgr/packages/default.inc for more examples.

Goals
=====
This will hopefully evolve into a full featured installer, which will allow multistep forms, and tasks, as well as module dependency checking, and be able to read settings/configure hooks from modules themselves after enabling them. 

