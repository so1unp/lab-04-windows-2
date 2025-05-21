### Condicion de carrera

Ocurre porque varios filosofos intentan agarrar el tenedor al mismo tiempo, esto ocurre porque no hay ningun mecanismo de sincronizacion para que esto no suceda. Por esto los filosofos pueden tomar el tenedor simultaneamente.

### Evitar la condicion (explicacion de apunte)

Para evitarlo hay que agregar un mutex en cada tenedor
**mutex**: Mecanimsmo de sincronización para evitar que varios hilos accedan simultáneamente a un recurso compartido.

##### **¿Como funciona?**

**Bloqueo (lock)**: Cuando un hilo quiere usar un recurso protegido por un mutex, debe "bloquear" el mutex (pthread_mutex_lock). Si el mutex está libre, el hilo lo bloquea y puede usar el recurso. Si ya está bloqueado por otro hilo, el hilo que intenta bloquearlo se queda esperando hasta que el mutex se libere.
**Desbloqueo (unlock)**: Cuando el hilo termina de usar el recurso, debe "desbloquear" el mutex (pthread_mutex_unlock). Así, otros hilos que estaban esperando pueden acceder al recurso.

### Evitar el bloqueo mutuo (deadlock)

El bloqueo mutuo ocurre si todos los filósofos toman primero el tenedor izquierdo y esperan por el derecho, generando un ciclo de espera.
La solu: hacer que uno de los filósofos (por ejemplo, el último) tome primero el tenedor derecho y luego el izquierdo
