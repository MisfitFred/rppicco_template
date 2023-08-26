.. loserCE documentation master file, created by
   sphinx-quickstart on Tue Apr 18 08:15:38 2023.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to loserCE's documentation!
===================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   z_chapter




Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

Docs
=================

.. namespace '::' className
.. doxygenclass:: chessInterface::uci
   :project: loserCE
   :members:

Headline
========

lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod
tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At
vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren,
no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit
amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut
labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam


.. code-block:: cpp
   :linenos:

   #include <iostream>
   #include <string>

   int main()
   {
       std::cout << "Hello World!" << std::endl;
       return 0;
   }

.. uml::

   class command {
       +execute()
   }
   class concreteCommand {
       +execute()
   }
   class invoker {
       +storeCommand()
   }
   class receiver {
       +action()
   }

   invoker -> command: execute()
   command -> receiver: action()
   receiver -> invoker: storeCommand()
   


       