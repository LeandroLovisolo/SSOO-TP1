((promedio=0))
for iter in {1..10000}
do
	((seed= $(rand) * $iter))
	temp=$(./simusched ejercicio9/tareas.tsk 1 0 0 SchedLottery 2 $seed | grep EXIT | awk '{ print $2 }')
	ultima=$(echo $temp | awk '{print $2}')
	primera=$(echo $temp | awk '{print $1}')
	promedio=$(echo "scale=2; ($promedio*($iter-1) + $ultima - $primera)/$iter" | bc)
	echo $iter,$promedio
done
