#!/bin/sh
# a minimal ERP to manage our PV installation operations
# need: bash and bc
for i in bin/*;do $i help;echo;done|less
exit 0

Use cases

* Acquire lead
  client add LEADNAME ==> CLIENT_ID
* Visit site
  installation add SITENAME
  installation set SITENAME client CLIENT_ID
* Submit offer
  installation devis SITENAME PRICE DOCUMENT
* Accept offer
  installation accept SITENAME
* Invoice customer
  client invoice LEADNAME (or CLIENT_ID) AMOUNT TEXT
* Clear customer's payment
  client payment LEADNAME (or CLIENT_ID) AMOUNT
* order stuff
  provider add PROVIDERNAME
  provider payment PROVIDERNAME AMOUNT
* receive stuff
  provider invoice PROVIDERNAME AMOUNT TEXT
* See stuff
  client compte LEADNAME
* report fincancials
  compta bilan
  compta journaux
  compta comptes
* record oil buy
  vehicule add CARNAME
  vehicule carburant CARNAME PRICE KM
* ...
  

Roadmap

* extend use cases when use arises
* expose object's methods via HTTP REST API
  (still in bash only with help of xinetd)
